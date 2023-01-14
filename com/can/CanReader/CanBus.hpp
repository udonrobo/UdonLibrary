/// @file   CanBus.hpp
/// @date   2023/01/13
/// @brief  CANバスを管理するクラス群
/// @author 大河 祐介

/// @remark 通信用ライブラリが関数コールバック方式をとっており、静的でないメンバ関数を登録できないため、メンバ関数が全てstaticになっています。
///         テンプレートメタプログラミングの仕様を利用して、staticメンバがある場合でもオブジェクト指向的に記述することができます。

#pragma once

#include <FlexCAN_T4.h>      // https://github.com/tonton81/FlexCAN_T4
#include <IntervalTimer.h>   // teensy standard lib

#include "list.hpp"  // std::list

/// @brief FlexCAN_T4ライブラリを用いたCANバス管理クラス
/// @tparam {Bus} バス種類[CAN0,CAN1,CAN2,CAN3]
/// @dataflow Teensy --CAN[H/L]--> CANBUS

template<CAN_DEV_TABLE Bus>
class CanBus {

		/// @brief ノード(Reader,Writer)を管理
		struct Node {
			uint16_t id;           // ノードID
			uint8_t* buffer;       // Reader,Writerのバッファを指すポインタ
			size_t length;         // バッファ長
			bool* instanceAlived;  // ノードが生存しているか(ノードがデストラクトされるとfalse)
		};

		FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;

		container::list<Node> readers;
		container::list<Node> writers;

		IntervalTimer writerThread;
		IntervalTimer readerThread;

		static CanBus* self;

	public:

		CanBus() {
			self = this;
		}

		~CanBus() {
			for (auto && it : readers) {
				delete it.instanceAlived;
			}
			for (auto && it : writers) {
				delete it.instanceAlived;
			}
		}

		/// @brief Readerクラスを登録
		/// @param {id} ノード識別子
		/// @param {buffer} バッファバイト列
		/// @return Readerのインスタンスが存在しているかを取得するためのフラグ(メモリ所有権はこのクラスにあります)
		/// @remark Readerのデストラクタでポインタ先にfalseを代入してください
		template<size_t N>
		bool* joinReader(uint16_t id, uint8_t (&buffer)[N]) {
			auto p = new bool(true);
			readers.push_back({ id, buffer, sizeof buffer, p });
			return p;
		}

		/// @brief Writerクラスを登録
		/// @param {id} ノード識別子
		/// @param {buffer} バッファバイト列
		/// @return Writerのインスタンスが存在しているかを取得するためのフラグ(メモリ所有権はこのクラスにあります)
		/// @remark Writerのデストラクタでポインタ先にfalseを代入してください
		template<size_t N>
		bool* joinWriter(uint16_t id, uint8_t (&buffer)[N]) {
			auto p = new bool(true);
			writers.push_back({ id, buffer, sizeof buffer, p });
			return p;
		}

		/// @brief 通信を開始
		/// @param {baudrate} 通信レート
		void begin(const uint32_t baudrate = 1000000) {

			Serial.println(readers.size());
			Serial.println(writers.size());
			if (readers.size() || writers.size())
			{
				// 通信開始
				bus.begin();
				bus.setBaudRate(baudrate);
				bus.enableFIFO();
				bus.enableFIFOInterrupt();
			}
			if (readers.size())
			{
				enableReaderIntterrupt();
			}
			if (writers.size())
			{
				enableWriterIntterrupt();
			}
		}

		void enableReaderIntterrupt() {

			// 受信割り込み開始
			bus.onReceive([](const CAN_message_t& msg) {
				const auto event = [&msg](auto & reader)
				{
					// 先頭1バイト : パケット番号
					const uint8_t index = msg.buf[0];

					// 8バイト受信データをバイト列にデコード
					for (uint8_t i = 0; i < 7; i++)
					{
						const uint8_t bufIndex = i + index * 7;
						if (bufIndex < reader.length)
							reader.buffer[bufIndex] = msg.buf[i + 1];
						else
							break;
							Serial.print(msg.buf[i + 1]);
					}
				};
				for (auto && it = self->readers.begin(); it != self->readers.end(); )
				{
					if (msg.id == (*it).id)
					{
						if ((*it).instanceAlived)
						{
							event(*it);
							++it;
						}
						else
						{
							// インスタンスが存在しない場合破棄
							delete (*it).instanceAlived;
							it = self->readers.erase(it);
						}
					}
				}
			});

			// 受信スレッド立ち上げ
			readerThread.begin(
			    [] { self->bus.events(); },
			    1000
			);
		}

		void enableWriterIntterrupt() {

			// 送信スレッド立ち上げ
			writerThread.begin(
			[] {
				const auto event = [](auto & reader) {
					// 一度に8バイトしか送れないため、パケットに分割し送信
					CAN_message_t msg;
					for (size_t index = 0; index < ceil(reader.length / 7.0); index++)
					{
						// 先頭1バイト : パケット番号
						msg.buf[0] = index;
						// バイト列を8バイト受信データにエンコード
						for (uint8_t i = 0; i < 7; i++)
						{
							const uint8_t bufIndex = i + index * 7;
							if (bufIndex < reader.length)
								msg.buf[i + 1] = reader.buffer[bufIndex];
							else
								break;
						}
					}
					// バスに送信
					while (!self->bus.write(msg));
				};
				for (auto && it = self->writers.begin(); it != self->writers.end(); )
				{
					if ((*it).instanceAlived)
					{
						event(*it);
						++it;
					}
					else
					{
						// インスタンスが存在しない場合破棄
						delete (*it).instanceAlived;
						it = self->writers.erase(it);
					}
				}
			},
			10000
			);

		}

};

template<CAN_DEV_TABLE Bus> CanBus<Bus>* CanBus<Bus>::self;
