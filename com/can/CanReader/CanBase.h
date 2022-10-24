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
		template<class T, size_t N>
		class Array {
				T value[N];
			public:
				/// @brief 変換コンストラクタ
				Array(const T* r) noexcept { memcpy(value, r, N); }
				const T& operator[](size_t index) const { return value[index]; }
				T& operator[](size_t index) { return value[index]; }
		};
		struct Message_t {
			uint8_t id;
			uint8_t index;
			Array<uint8_t, 8> data;
		};

#if USE_TEENSY
		using Can = FlexCAN_T4<CAN_BAS, RX_SIZE_256, TX_SIZE_256>;
#else
		using Can = MCP2515;
		static constexpr uint8_t interruptPin = 2;
		static constexpr uint8_t csPin = 10;
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
				FunctionBinder_t::bind({                           /// ---- const Message_t& ----
					static_cast<uint8_t>(msg.id      &     0b111), /// uint8_t           id
					static_cast<uint8_t>(msg.id >> 7 & 0b1111111), /// uint8_t           index
					msg.buf,                                       /// Array<uint8_t, 8> data
				});
			});
			static IntervalTimer timer;
			timer.begin([] { can.events(); }, 200);
#	endif

#else  /// not teensy
			can.reset();
			can.setBitrate(CAN_1000KBPS);
			can.setNormalMode();
#	if USE_READER
			pinMode(interruptPin, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(interruptPin), [] {
				can_frame msg;
				if (can.readMessage(&msg) == MCP2515::ERROR_OK) {
					FunctionBinder_t::bind({
						static_cast<uint8_t>(msg.can_id & 0b111),          /* uint8_t id; */
						static_cast<uint8_t>(msg.can_id >> 7 & 0b1111111), /* uint8_t index; */
						msg.data
					});
				}
			}, CHANGE);
#	endif

#endif
		}  /// begin()

		/// @brief 送信処理
		/// @param msg 送信内容
		static void write(const Message_t& msg) {
#if USE_TEENSY
			CAN_message_t output = { static_cast<uint32_t>(msg.index) << 7 | static_cast<uint8_t>(msg.id) };
			memcpy(output.buf, msg.buf, 8);
			//			for (uint8_t i = 0; i < 100; i++)
			//				if (can.write(output))
			//					break;
			while (!can.write(output));
#else
			can_frame output = { static_cast<uint32_t>(static_cast<uint32_t>(msg.index) << 7 | msg.id) };
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
