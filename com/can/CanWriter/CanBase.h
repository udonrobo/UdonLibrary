/// @file   CanBase.h
/// @date   2022/09/27
/// @brief  CAN通信基底クラス
/// @author 大河 祐介

#pragma once

#define SUPPORTED_TEENSY                                      \
	defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY40) || \
	defined(ARDUINO_TEENSY36) || defined(ARDUINO_TEENSY35) || \
	defined(ARDUINO_TEENSY32) || defined(ARDUINO_TEENSY30)

#define WITH_READER __has_include("CanReader.h")

#if SUPPORTED_TEENSY
#	include <FlexCAN_T4.h>  /// https://github.com/tonton81/FlexCAN_T4
#	include <IntervalTimer.h>
#else
#	include <mcp2515.h>     /// https://github.com/autowp/arduino-mcp2515
#endif

#if WITH_READER
#	include "FunctionBinder.h"
#endif

template <class Dum>  /// リンクエラー対策
class _CanBase {
	protected:
	
		struct Message_t {
			uint32_t id;
			uint8_t buf[8];
		};

#if SUPPORTED_TEENSY
		using Can = FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256>;
#else
		static constexpr uint8_t interruptPin = 2;
		static constexpr uint8_t csPin = 10;
		using Can = MCP2515;
#endif

		static Can can;
		static uint8_t id;

		static void begin() {
#if SUPPORTED_TEENSY
			can.begin();
			can.setClock(CLK_60MHz);
			can.setBaudRate(1000000);
			can.enableFIFO();
			can.enableFIFOInterrupt();
#	if WITH_READER
			can.onReceive([](const CAN_message_t& input) {
				Message_t msg = { input.id };
				memcpy(msg.buf, input.buf, 8);
				FunctionBinder<const Message_t&>::bind(msg);
			});
			static IntervalTimer timer;
			timer.begin([] { can.events(); }, 200);
#	endif
#else
			can.reset();
			can.setBitrate(CAN_1000KBPS);
			can.setNormalMode();
#	if WITH_READER
			pinMode(interruptPin, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(interruptPin), [] {
				can_frame input;
				if (can.readMessage(&input) == MCP2515::ERROR_OK) {
					Message_t msg = { input.can_id };
					memcpy(msg.buf, input.data, 8);
					FunctionBinder<const Message_t&>::bind(msg);
				}
			}, CHANGE);
#	endif

#endif
		}  /// begin()

		/// @brief 送信処理
		/// @param msg 送信内容
		static void write(const Message_t& msg) {
#if SUPPORTED_TEENSY
			CAN_message_t output = { msg.id };
			memcpy(output.buf, msg.buf, 8);
			while (!can.write(output));
#else
			can_frame output = { msg.id };
			memcpy(output.data, msg.buf, 8);
			can.sendMessage(&output);
#endif
		}
};

using CanBase = _CanBase<void>;

#if SUPPORTED_TEENSY
template <class Dum>CanBase::Can _CanBase<Dum>::can;
#else
template <class Dum>CanBase::Can _CanBase<Dum>::can(CanBase::csPin);
#endif

template <class Dum>uint8_t  _CanBase<Dum>::id;
