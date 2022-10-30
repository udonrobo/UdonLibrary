/// @file   CanBase.h
/// @date   2022/09/27
/// @brief  CAN通信基底クラス
/// @author 大河 祐介

#pragma once


#define USE_TEENSY_4X defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY40)
#define USE_TEENSY_3X defined(ARDUINO_TEENSY36) || defined(ARDUINO_TEENSY35)
#define USE_TEENSY    USE_TEENSY_3X || USE_TEENSY_4X

#ifdef USE_TEENSY_4X
#	define CAN_BAS CAN1
#elif USE_TEENSY_3X
#	define CAN_BAS CAN0
#endif

#if USE_TEENSY
#	include <FlexCAN_T4.h>  /// https://github.com/tonton81/FlexCAN_T4
#	include <IntervalTimer.h>
#else
#	include <mcp2515.h>     /// https://github.com/autowp/arduino-mcp2515
#endif

/// 受信割り込みを行うか判別する
#define USE_READER __has_include("CanReader.h")

#if USE_READER
#	include "FunctionBinder.h"
#endif


template <class Dum>  /// リンクエラー対策
class _CanBase {
	protected:
		struct Message_t {
			uint32_t& id   ;  /// data id
			uint8_t&  index;  /// packet index
			uint8_t*  data ;  /// message array[7]
			constexpr static uint8_t dataLength = 7;
		};

#if USE_TEENSY
		using Can = FlexCAN_T4<CAN_BAS, RX_SIZE_256, TX_SIZE_256>;
#else
		using Can = MCP2515;
		static constexpr uint8_t interruptPin = 2;
		static constexpr uint8_t csPin        = 10;
#endif

#if USE_READER
		using FunctionBinder_t = FunctionBinder<void(const Message_t&)>;
#endif
		static Can can;

		static void begin() {
#if USE_TEENSY
			can.begin();
			//			can.setClock(CLK_60MHz);
			can.setBaudRate(1000000);
			can.enableFIFO();
			can.enableFIFOInterrupt();
#	if USE_READER
			can.onReceive([](const CAN_message_t& msg) {
				FunctionBinder_t::bind({
					const_cast<uint32_t&>(msg.id     ),  /// _In_ uint8_t id
					const_cast<uint8_t& >(msg.buf[0] ),  /// _In_ uint8_t index
					const_cast<uint8_t* >(msg.buf + 1)   /// _In_ uint8_t data[7]
				});
			});
			static IntervalTimer timer;
			timer.begin([] { can.events(); }, 100);
#	endif

#else
			can.reset();
			can.setBitrate(CAN_1000KBPS);
			can.setNormalMode();
#	if USE_READER
			pinMode(interruptPin, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(interruptPin), [] {
				can_frame msg;
				if (can.readMessage(&msg) == MCP2515::ERROR_OK) {
					FunctionBinder_t::bind({
						const_cast<uint32_t&>(msg.can_id  ),  /// _In_ uint8_t id
						const_cast<uint8_t& >(msg.data[0] ),  /// _In_ uint8_t index
						const_cast<uint8_t* >(msg.data + 1)   /// _In_ uint8_t data[7]
					});
				}
			}, CHANGE);
#	endif

#endif
		}

		/// @brief 送信処理
		static void write(void (*callback)(Message_t&&, void*), void* _this) {
#if USE_TEENSY
			CAN_message_t libmsg;
			callback({
				libmsg.id     ,  /// _Out_ uint8_t id
				libmsg.buf[0] ,  /// _Out_ uint8_t index
				libmsg.buf + 1,  /// _Out_ uint8_t data[7]
			}, _this);
			while (!can.write(libmsg));
#else
			can_frame libmsg;
			callback({
				libmsg.id     ,  /// _Out_ uint8_t id
				libmsg.buf[0] ,  /// _Out_ uint8_t index
				libmsg.buf + 1,  /// _Out_ uint8_t data[7]
			}, _this);
			can.sendMessage(&libmsg);
#endif
		}
};

using CanBase = _CanBase<void>;

#if USE_TEENSY
template <class Dum>CanBase::Can _CanBase<Dum>::can;
#else
template <class Dum>CanBase::Can _CanBase<Dum>::can(CanBase::csPin);
#endif

#undef USE_TEENSY_4X
#undef USE_TEENSY_3X
#undef USE_TEENSY
#undef CAN_BAS
#undef USE_READER
