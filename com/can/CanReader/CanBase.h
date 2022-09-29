/// @file   CanBase.h
/// @date   2022/09/27
/// @brief  CAN通信基底クラス
/// @author 大河 祐介

#pragma once

#define TEENSY_4X defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY40)
#define TEENSY_3X defined(ARDUINO_TEENSY36) || defined(ARDUINO_TEENSY35)
#define USE_TEENSY TEENSY_3X || TEENSY_4X

/// サポートされているCANバスに切り替え
#ifdef TEENSY_4X
#	define CAN_BAS CAN1
#elif TEENSY_3X
#	define CAN_BAS CAN0
#endif

#if USE_TEENSY
#	include <FlexCAN_T4.h>  /// https://github.com/tonton81/FlexCAN_T4
#	include <IntervalTimer.h>
#else
#	include <mcp2515.h>     /// https://github.com/autowp/arduino-mcp2515
#endif

/// 受信割り込みを行うか判別する
#define WITH_READER __has_include("CanReader.h")

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

#if USE_TEENSY
		using Can = FlexCAN_T4<CAN_BAS, RX_SIZE_256, TX_SIZE_256>;
#else
		using Can = MCP2515;
		static constexpr uint8_t interruptPin = 2;
		static constexpr uint8_t csPin = 10;
#endif

#if WITH_READER
		using FunctionBinder_t = FunctionBinder<const Message_t&>;
#endif

		static Can can;

		static void begin() {
#if USE_TEENSY
			can.begin();
			//			can.setClock(CLK_60MHz);
			can.setBaudRate(1000000);
			can.enableFIFO();
			can.enableFIFOInterrupt();
#	if WITH_READER
			can.onReceive([](const CAN_message_t& input) {
				Message_t msg = { input.id };
				memcpy(msg.buf, input.buf, 8);
				FunctionBinder_t::bind(msg);
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
					FunctionBinder_t::bind(msg);
				}
			}, CHANGE);
#	endif

#endif
		}  /// begin()

		/// @brief 送信処理
		/// @param msg 送信内容
		static void write(const Message_t& msg) {
#if USE_TEENSY
			CAN_message_t output = { msg.id };
			memcpy(output.buf, msg.buf, 8);
			//			for (uint8_t i = 0; i < 100; i++)
			//				if (can.write(output))
			//					break;
			while (!can.write(output));
#else
			can_frame output = { msg.id };
			memcpy(output.data, msg.buf, 8);
			can.sendMessage(&output);
#endif
		}
};

using CanBase = _CanBase<void>;

#if USE_TEENSY
template <class Dum>CanBase::Can _CanBase<Dum>::can;
#else
template <class Dum>CanBase::Can _CanBase<Dum>::can(CanBase::csPin);
#endif
