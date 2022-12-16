#pragma once

#define USE_TEENSY_4X defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY40)
#define USE_TEENSY_3X defined(ARDUINO_TEENSY36) || defined(ARDUINO_TEENSY35)
#define USE_TEENSY    USE_TEENSY_3X || USE_TEENSY_4X

#if USE_TEENSY
#	include <FlexCAN_T4.h>  /// https://github.com/tonton81/FlexCAN_T4
#	include <IntervalTimer.h>
#else
#	include <mcp2515.h>     /// https://github.com/autowp/arduino-mcp2515
#endif

#include "ForwardLinearList.hpp"

#if USE_TEENSY

/// Chip --> CAN Transceiver --> CAN BUS

template<CAN_DEV_TABLE BusTy>
class CanBus {

		FlexCAN_T4<BusTy, RX_SIZE_256, TX_SIZE_256> bus;
		IntervalTimer timer;

		struct Mail {
			uint16_t id;
			uint8_t* buffer;
			size_t   bufferLength;
		};

		ForwardLinearList<Mail> mailBox;
		bool isBegined;

	public:
		enum class BeginSatus : uint8_t {
			EnableReadIntrrupt
		};

		CanBus() {
		}

		template<size_t Size>
		void appendMailBox(uint16_t id, uint8_t (&buffer)[Size]) {
			mailBox.push_back({ id, buffer, Size });
		}

		/// @brief 通信を開始していない場合開始する
		/// @param status 開始
		void begin() {

			if (isBegined) return;
			bus.begin();
			bus.setClock(CLK_60MHz);
			bus.setBaudRate(1000000);
			bus.enableFIFO();
			bus.enableFIFOInterrupt();
			//			timer.begin(can.events, 200);
			isBegined = true;
		}
};

#elif

/// Chip --> CAN Controller --> CAN Transceiver --> CAN BUS

class CanBus {
		MCP2515 bus;
		struct Mail {
			uint16_t id;
			uint8_t* buffer;
			size_t   bufferLength;
		};
		ForwardLinearList<Mail> mailBox;
		bool isBegined;
	public:
		enum class BeginSatus : uint8_t {
			EnableReadIntrrupt
		};

		CanBus(const uint8_t cspin) {
		}

		template<size_t Size>
		void appendMailBox(uint16_t id, uint8_t (&buffer)[Size]) {
			mailBox.push_back({ id, buffer, Size });
		}

		/// @brief 通信を開始していない場合開始する
		/// @param status 開始
		void begin() {

			if (isBegined) return;
			bus.reset();
			bus.setBitrate(CAN_1000KBPS);
			bus.setNormalMode();
			if (status == BeginSatus::EnableReadIntrrupt) {
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
			}
			isBegined = true;
		}
};

#endif
