/// @file   CanBusTeensy.hpp
/// @date   2023/01/13
/// @brief  FlexCAN_T4ライブラリを用いたCANバス管理クラス
/// @flow   [CPU] <--CAN[TX/RX]--> [CANトランシーバ] <--CAN[H/L]--> [BUS]
/// @author 大河 祐介

#pragma once

#include <FlexCAN_T4.h>      // https://github.com/tonton81/FlexCAN_T4.git
#include <IntervalTimer.h>   // https://github.com/loglow/IntervalTimer.git

#include "list.hpp"
#include "memory.hpp"

/// @tparam {Bus} バス種類 (CAN0,CAN1,CAN2,CAN3)
template<CAN_DEV_TABLE Bus>
class CanBusTeensy {

		FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;
		static CanBusTeensy* self;

		IntervalTimer readerIsr;  // 送信割り込み用タイマー
		uint32_t lastWriteUs;     // 最終送信時刻

		/// @brief ノード(Reader,Writer)を管理
		struct Node {
			uint16_t  id;              // ノードID
			uint8_t*  buffer;          // Reader,Writerクラスのバッファに対するポインタ
			size_t    size;            // バッファサイズ
			uint32_t* timestamp;       // 最後にバッファにアクセスした時刻[ms]
			udon::std::shared_ptr<bool> instanceAlived;  // ノードが生存しているか(ノードがデストラクトされるタイミングでfalse)
		};

		udon::std::list<Node> readers;
		udon::std::list<Node> writers;

	public:

		CanBusTeensy()
			: bus()
			, lastWriteUs()
			, readers()
			, writers()
		{
			self = this;
		}

		~CanBusTeensy() {
			readerIsr.end();
		}

		/// @brief 通信を開始
		/// @param {baudrate} 通信レート
		void begin(const uint32_t baudrate = 1000000)
		{
			if (readers.size() || writers.size())
			{
				bus.begin();
				bus.setBaudRate(baudrate);
				bus.enableFIFO();
				bus.enableFIFOInterrupt();
			}
			if (readers.size())
			{
				enableReaderInterrupt();
			}
		}

		/// @brief バスを更新
		/// @param {writeIntervalUs} 送信間隔
		void update(uint32_t writeIntervalUs = 5000)
		{
			const auto now = micros();
			if (writers.size() && now - lastWriteUs >= writeIntervalUs)
			{
				const auto event = [&now](Node & writer) {
					// 一度に8バイトしか送れないため、パケットに分割し送信
					for (size_t index = 0; index < ceil(writer.size / 7.0); index++)
					{
						CAN_message_t msg;
						
						// 先頭1バイト : パケット番号
						msg.id = writer.id;
						msg.buf[0] = index;
						
						// バイト列を8バイト受信データにエンコード
						for (uint8_t i = 0; i < 7; i++)
						{
							const uint8_t bufIndex = i + index * 7;

							if (bufIndex < writer.size)
								msg.buf[i + 1] = writer.buffer[bufIndex];
							else
								break;
						}
						// バスに送信
						while (!self->bus.write(msg));
					}
					*writer.timestamp = now;
				};
				for (auto && it = self->writers.begin(); it != self->writers.end(); )
				{
					// インスタンスが存在しない場合インスタンスの管理から解放
					if (*it->instanceAlived)
					{
						event(*it);
						++it;
					}
					else
					{
						it = self->writers.erase(it);
					}
				}
				lastWriteUs = now;
			}
		}

		/// @brief Readerクラスを登録
		/// @param {id} ノード識別子
		/// @param {buffer} バッファバイト列
		/// @return Readerのインスタンスが存在しているかを取得するためのフラグ(メモリ所有権はこのクラスにあります)
		/// @remark Readerのデストラクタでポインタ先にfalseを代入してください
		template<size_t N>
		auto joinReader(uint16_t id, uint8_t (&buffer)[N], uint32_t& timestamp)
		{
			udon::std::shared_ptr<bool> p(new bool(true));
			readers.push_back({ id, buffer, sizeof buffer, &timestamp, p });
			return p;
		}

		/// @brief Writerクラスを登録
		/// @param {id} ノード識別子
		/// @param {buffer} バッファバイト列
		/// @return Writerのインスタンスが存在しているかを取得するためのフラグ(メモリ所有権はこのクラスにあります)
		/// @remark Writerのデストラクタでポインタ先にfalseを代入してください
		template<size_t N>
		auto joinWriter(uint16_t id, uint8_t (&buffer)[N], uint32_t& timestamp)
		{
			udon::std::shared_ptr<bool> p(new bool(true));
			writers.push_back({ id, buffer, sizeof buffer, &timestamp, p });
			return p;
		}

	private:

		// @brief 受信割り込み開始
		void enableReaderInterrupt()
		{
			bus.onReceive([](const CAN_message_t& msg) {
				const auto event = [&msg](Node & reader)
				{
					// 先頭1バイト : パケット番号
					const uint8_t index = msg.buf[0];

					// 8バイト受信データをバイト列にデコード
					for (uint8_t i = 0; i < 7; i++)
					{
						const uint8_t bufIndex = i + index * 7;
						if (bufIndex < reader.size)
							reader.buffer[bufIndex] = msg.buf[i + 1];
						else
							break;
					}
					*reader.timestamp = millis();
				};
				for (auto && it = self->readers.begin(); it != self->readers.end(); )
				{
					// インスタンスが存在しない場合インスタンスの管理を解放
					if (*it->instanceAlived)
					{
						if (msg.id == it->id)
						{
							event(*it);
						}
						++it;
					}
					else
					{
						it = self->readers.erase(it);
					}
				}
			});

			// 受信割り込み開始
			readerIsr.begin(
			    [] { self->bus.events(); },
			    100
			);
		}

};

template<CAN_DEV_TABLE Bus>
CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;
