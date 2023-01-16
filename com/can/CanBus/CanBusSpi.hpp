/// @file   CanBusSpi.hpp
/// @date   2023/01/13
/// @brief  FlexCAN_T4ライブラリを用いたCANバス管理クラス
/// @flow   [CPU] <--SPI--> [CANコントローラ] <--CAN[TX/RX]--> [CANトランシーバ] <--CAN[H/L]--> [BUS]
/// @author 大河 祐介

#pragma once

#include <mcp2515.h>   // https://github.com/autowp/arduino-mcp2515

#include "list.hpp"
#include "memory.hpp"

template<uint8_t Cs, uint8_t Interrupt>
class CanBusSpi {

		MCP2515 bus;
		static CanBusSpi* self;

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

		/// @brief constructor
		/// @param {spi} SPIインスタンス
		/// @param {spiClock} SPI動作クロック
		/// @remark 動作クロックは CPUクロック/2 が最大値
		CanBusSpi(SPIClass& spi, uint32_t spiClock = 10000000)
			: bus(Cs, spiClock, &spi)
			, lastWriteUs()
			, readers()
			, writers()
		{
			self = this;
		}
		
		~CanBusSpi() {
		}

		/// @brief 通信を開始
		/// @param {baudrate} 通信レート
		void begin(CAN_SPEED baudrate = CAN_1000KBPS)
		{
			if (readers.size() || writers.size())
			{
				bus.reset();
				bus.setBitrate(baudrate);
				bus.setNormalMode();
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
				const auto event = [](Node & writer) {
					// 一度に8バイトしか送れないため、パケットに分割し送信
					for (size_t index = 0; index < ceil(writer.size / 7.0); index++)
					{
						can_frame msg;
						
						// 先頭1バイト : パケット番号
						msg.can_id = writer.id;
						msg.data[0] = index;
						
						// バイト列を8バイト受信データにエンコード
						for (uint8_t i = 0; i < 7; i++)
						{
							const uint8_t bufIndex = i + index * 7;

							if (bufIndex < writer.size)
								msg.data[i + 1] = writer.buffer[bufIndex];
							else
								break;
						}
						// バスに送信
						self->bus.sendMessage(&msg);
					}
					*writer.timestamp = millis();
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
			pinMode(Interrupt, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(Interrupt), [] {
				can_frame msg;
				if (self->bus.readMessage(&msg) == MCP2515::ERROR_OK) {
					const auto event = [&msg](Node & reader)
					{
						// 先頭1バイト : パケット番号
						const uint8_t index = msg.data[0];

						// 8バイト受信データをバイト列にデコード
						for (uint8_t i = 0; i < 7; i++)
						{
							const uint8_t bufIndex = i + index * 7;
							if (bufIndex < reader.size)
								reader.buffer[bufIndex] = msg.data[i + 1];
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
							if (msg.can_id == it->id)
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
				}
			}, CHANGE);

		}

};

template<uint8_t Cs, uint8_t Interrupt>
CanBusSpi<Cs, Interrupt>* CanBusSpi<Cs, Interrupt>::self;
