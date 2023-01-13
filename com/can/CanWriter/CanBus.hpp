/// @file   CanBus.hpp
/// @date   2023/01/13
/// @brief  CANバスを管理するクラス群
/// @author 大河 祐介

/// @remark 通信用ライブラリが関数コールバック方式をとっており、静的でないメンバ関数を登録できないため、メンバ関数が全てstaticになっています。
///         テンプレートメタプログラミングの仕様を利用して、staticメンバがある場合でもオブジェクト指向的に記述することができます。

#pragma once

#include <FlexCAN_T4.h>      // https://github.com/tonton81/FlexCAN_T4
#include <IntervalTimer.h>   // teensy standard lib

#include <list>  // std::list
#include <memory>

template<CAN_DEV_TABLE Bus>
class CanBus {

		/// @brief ノード(Reader,Writer)を管理するクラス
		struct Node {
			uint16_t id;           // ノードID
			uint8_t* buffer;       // Reader,Writerクラスのバッファに対するポインタ
			size_t length;         // バッファの長さ
			bool* instanceAlived;  // ノードが生存しているかどうか
		};

		static FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;

		static std::list<Node> readers;
		static std::list<Node> writers;

		static IntervalTimer writerThread;
		static IntervalTimer readerThread;

	public:

		~CanBus() {
			for(auto&& it : readers) {
				delete it->instanceAlived;
			}
			for(auto&& it : writers) {
				delete it->instanceAlived;
			}
		}


		template<size_t N>
		static void joinReader(uint16_t id, uint8_t (&buffer)[N], bool* instanceAlived) {
			readers.push_back({ id, buffer, sizeof buffer, instanceAlived });
		}

		template<size_t N>
		static void joinWriter(uint16_t id, uint8_t (&buffer)[N], std::shared_ptr<bool>) {
			writers.push_back({ id, buffer, sizeof buffer });
		}

		static void begin() {
			bus.begin();
			bus.setBaudRate(1000000);
			bus.enableFIFO();
			bus.enableFIFOInterrupt();
			bus.onReceive([](const CAN_message_t& msg) {
				const auto event = [&msg](auto & reader) {
					const uint8_t index = msg.buf[0];
					for (uint8_t i = 0; i < 7; i++) {
						const uint8_t bufIndex = i + index * 7;
						if (bufIndex < reader.length)
							reader.buffer[bufIndex] = msg.buf[i + 1];
						else
							break;
					}
				};
				for (auto && reader : readers) {
					if (msg.id == reader.id) {
						event(reader);
					}
				}
			});

			writerThread.begin(
			[] {
				const auto event = [](auto & reader) {
					CAN_message_t msg;
					for (size_t index = 0; index < ceil(reader.length / 7.0); index++) {
						msg.buf[0] = index;
						for (uint8_t i = 0; i < 7; i++) {
							const uint8_t bufIndex = i + index * 7;
							if (bufIndex < reader.length)
								msg.buf[i + 1] = reader.buffer[bufIndex];
							else
								break;
						}
					}
					while (!bus.write(msg));
				};
				for (auto && reader : readers) {
					event(reader);
				}
			},
			10000
			);
			readerThread.begin(
			    [] { bus.events(); },
			    100
			);
		}

};

template<CAN_DEV_TABLE Bus> FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> CanBus<Bus>::bus;
template<CAN_DEV_TABLE Bus> std::list<typename CanBus<Bus>::Node> CanBus<Bus>::readers;
template<CAN_DEV_TABLE Bus> std::list<typename CanBus<Bus>::Node> CanBus<Bus>::writers;
template<CAN_DEV_TABLE Bus> IntervalTimer CanBus<Bus>::writerThread;
template<CAN_DEV_TABLE Bus> IntervalTimer CanBus<Bus>::readerThread;

template<uint8_t Bus>
class CanFdBus {
};


#include <SPI.h>

struct Info {

};

template<class Ty, Ty info>
class Hoge {

};

//extern SPIClass spi;

//template<SPIClass Spi>
//class CanSpiBus {
//};
