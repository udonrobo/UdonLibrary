/// @file   CanBase.h
/// @date   2022/09/27
/// @brief  CAN通信基底クラス
/// @author 大河 祐介

#pragma once

<<<<<<< .merge_file_a36232
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
=======
#	include <FlexCAN_T4.h>  /// https://github.com/tonton81/FlexCAN_T4
#	include <IntervalTimer.h>
>>>>>>> .merge_file_a12340

#include "FunctionBinder.h"

template <class Dum>  /// リンクエラー対策
class _CanBase {
	protected:
		template<class T, size_t N>
		class Array {
				T value[N];
			public:
				/// @brief 変換コンストラクタ
<<<<<<< .merge_file_a36232
				Array(const T* r) noexcept { memcpy(value, r, N); }
				const T& operator[](size_t index) const { return value[index]; }
=======
				Array(const T* r) noexcept {
					memcpy(value, r, N);
				}
				const T& operator[](size_t index) const {
					return value[index];
				}
>>>>>>> .merge_file_a12340
		};
		struct Message_t {
			uint8_t signalId;
			uint8_t packetId;
			Array<uint8_t, 8> data;
		};
<<<<<<< .merge_file_a36232

#if USE_TEENSY
		using Can = FlexCAN_T4<CAN_BAS, RX_SIZE_256, TX_SIZE_256>;
#else
		using Can = MCP2515;
		static constexpr uint8_t interruptPin = 2;
		static constexpr uint8_t csPin = 10;
#endif

#if USE_READER
//		using FunctionBinder_t = FunctionBinder<void(const Message_t&)>;
#endif
=======

		using Can = FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256>;
		static Can can;

		using FunctionBinder_t = FunctionBinder<void(const Message_t&)>;
>>>>>>> .merge_file_a12340


		static void begin() {
			can.begin();
			//			can.setClock(CLK_60MHz);
			can.setBaudRate(1000000);
			can.enableFIFO();
			can.enableFIFOInterrupt();
<<<<<<< .merge_file_a36232
#	if USE_READER
			can.onReceive([](const CAN_message_t& input) {
				FunctionBinder<void(const Message_t&)>::bind({                             /// ---- const Message_t& ----
					static_cast<uint8_t>(input.id      &     0b111), /// uint8_t           signalId
					static_cast<uint8_t>(input.id >> 7 & 0b1111111), /// uint8_t           packetId
					input.buf,                                       /// Array<uint8_t, 8> data    
=======
			can.onReceive([](const CAN_message_t& msg) {
				FunctionBinder_t::bind({                           /// ---- const Message_t& ----
					static_cast<uint8_t>(msg.id      &     0b111), /// uint8_t           signalId
					static_cast<uint8_t>(msg.id >> 7 & 0b1111111), /// uint8_t           packetId
					msg.buf,                                       /// Array<uint8_t, 8> data
>>>>>>> .merge_file_a12340
				});
			});
			static IntervalTimer timer;
			timer.begin([] { can.events(); }, 200);
<<<<<<< .merge_file_a36232
#	endif
#else
			can.reset();
			can.setBitrate(CAN_1000KBPS);
			can.setNormalMode();
#	if USE_READER
			pinMode(interruptPin, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(interruptPin), [] {
				can_frame input;
				if (can.readMessage(&input) == MCP2515::ERROR_OK) {
					Message_t msg = {
						static_cast<uint8_t>(input.can_id & 0b111),          /* uint8_t signalId; */
						static_cast<uint8_t>(input.can_id >> 7 & 0b1111111), /* uint8_t packetId; */
					};
					memcpy(msg.buf, input.data, 8);
					FunctionBinder_t::bind(msg);
				}
			}, CHANGE);
#	endif

#endif
		}  /// begin()
=======
		}
>>>>>>> .merge_file_a12340

		/// @brief 送信処理
		/// @param msg 送信内容
		static void write(const Message_t& msg) {
			CAN_message_t output = { static_cast<uint32_t>(msg.packetId) << 7 | static_cast<uint8_t>(msg.signalId) };
			memcpy(output.buf, msg.buf, 8);
			while (!can.write(output));
		}
};

using CanBase = _CanBase<void>;
template <class Dum>CanBase::Can _CanBase<Dum>::can;
