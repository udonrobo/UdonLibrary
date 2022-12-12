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

template<int Bus>
class CanBus {

#if USE_TEENSY
		FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> can;
#else
		MCP2515 can;
#endif

		bool isBegined;
		IntervalTimer timer;

	public:
		enum class BeginSatus : uint8_t {
			EnableReadIntrrupt
		};

#if USE_TEENSY
		CanBus(const uint8_t cs) {

		}
#else
		CanBus() {

		}
#endif

		/// @brief 通信を開始していない場合開始する
		/// @param status 開始
		void begin(BeginSatus status = {}) {

			if (isBegined) return;
#ifdef USE_TEENSY
			can.begin();
			can.setClock(CLK_60MHz);
			can.setBaudRate(1000000);
			can.enableFIFO();
			can.enableFIFOInterrupt();
			if (status == BeginSatus::EnableReadIntrrupt) {
				timer.begin([&] { can.events(); }, 200);
			}
#elif
			can.reset();
			can.setBitrate(CAN_1000KBPS);
			can.setNormalMode();
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
#endif

			isBegined = true;
		}

};
