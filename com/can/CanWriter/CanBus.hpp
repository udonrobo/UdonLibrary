#pragma once

class BusInterface {

};


template<uint8_t Bus>
class CanBus {
		struct Node {
			uint16_t id;
			uint8_t* buffer;
			size_t length;
		};

		static FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;
		static std::list<Node> readers;
		static std::list<Node> writers;

		static IntervalTimer writerThread;
		static IntervalTimer readerThread;

	public:
		template<size_t N>
		static void joinReader(uint16_t id, uint8_t (&buffer)[N]) {
			readers.push_back({ id, buffer, sizeof buffer });
		}

		template<size_t N>
		static void joinWriter(uint16_t id, uint8_t (&buffer)[N]) {
			writers.push_back({ id, buffer, sizeof buffer });
		}

		static void begin() {
			can.begin();
			can.setBaudRate(1000000);
			can.enableFIFO();
			can.enableFIFOInterrupt();
			can.onReceive([](const CAN_message_t& msg) {
				const auto event = [&msg] {
					const uint8_t index = msg.buf[0];
					for (uint8_t i = 0; i < 7; i++) {
						const uint8_t bufIndex = i + index * 7;
						if (bufIndex < Size)
							buffer[bufIndex] = msg.buf[i + 1];
						else
							break;
					}
				};
				for (auto && it : readers) {
					if (msg.id == it.id) {
						event();
					}
				}
			});
			writerThread.begin(
			[] {
				for (auto && it : readers) {
					CAN_message_t msg;
					for (size_t index = 0; index < ceil(it.length / 7.0); index++) {
						msg.data[0] = index;
						for (uint8_t i = 0; i < 7; i++) {
							const uint8_t bufIndex = i + index * 7;
							if (bufIndex < Size)
								msg.data[i + 1] = it.buffer[bufIndex];
							else
								break;
						}
					}
					while (!can.write(msg));
				}
			},
			10000
			);
			readerThread.begin(
			    [] { can.events(); },
			    100
			);
		}

};


template<uint8_t Bus>
class CanFdBus {
};


template<uint8_t Cs, uint8_t Interrupt>
class CanSpiBus {

		static void begin() {
		}
		static void joinWriterBuffer() {
		}


};
