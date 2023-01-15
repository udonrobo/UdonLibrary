/// @file   CanBusSpi.hpp
/// @date   2023/01/13
/// @brief  FlexCAN_T4ライブラリを用いたCANバス管理クラス
/// @flow   [CPU] <--SPI--> [CANコントローラ] <--CAN[TX/RX]--> [CANトランシーバ] <--CAN[H/L]--> [BUS]
/// @author 大河 祐介

#pragma once

#include <mcp2515.h>   // https://github.com/autowp/arduino-mcp2515

#include "list.hpp"

template<uint8_t Cs, uint8_t Interrupt>
class CanBusSpi {

		MCP2515 bus;
		static CanBusSpi* self;

		/// @brief ノード(Reader,Writer)を管理
		struct Node {
			uint16_t  id;              // ノードID
			uint8_t*  buffer;          // Reader,Writerクラスのバッファに対するポインタ
			size_t    size;            // バッファサイズ
			uint32_t* timestamp;       // 最後にバッファにアクセスした時刻[ms]
			bool*     instanceAlived;  // ノードが生存しているか(ノードがデストラクトされるタイミングでfalseになります)
		};
		container::list<Node> readers;
		container::list<Node> writers;
		
	public:

		/// @brief constructor
		/// @param {spi} SPIインスタンス
		/// @param {spiClock} SPI動作クロック
		/// @remark 動作クロックは CPUクロック/2 が最大値
		CanBusSpi(SPIClass& spi, uint32_t spiClock = 10000000)
			: bus(Cs, spiClock, &spi)
		{
			self = this;
		}

		~CanBusSpi() {
			for (auto && it : readers)
			{
				delete it.instanceAlived;
			}
			for (auto && it : writers)
			{
				delete it.instanceAlived;
			}
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
				enableReaderIntterrupt();
			}
		}

		/// @brief バスを更新
		void update()
		{
			if (writers.size())
			{
				const auto event = [](Node& writer) {
					// 一度に8バイトしか送れないため、パケットに分割し送信
					for (size_t index = 0; index < ceil(writer.size / 7.0); index++)
					{
						can_frame msg;
						// 先頭1バイト : パケット番号
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
					if (it->instanceAlived)
					{
						event(*it);
						++it;
					}
					else
					{
						// インスタンスが存在しない場合破棄
						delete it->instanceAlived;
						it = self->writers.erase(it);
					}
				}
			}
		}

		/// @brief Readerクラスを登録
		/// @param {id} ノード識別子
		/// @param {buffer} バッファバイト列
		/// @return Readerのインスタンスが存在しているかを取得するためのフラグ(メモリ所有権はこのクラスにあります)
		/// @remark Readerのデストラクタでポインタ先にfalseを代入してください
		template<size_t N>
		bool* joinReader(uint16_t id, uint8_t (&buffer)[N], uint32_t& timestamp)
		{
			auto p = new bool(true);
			readers.push_back({ id, buffer, sizeof buffer, &timestamp, p });
			return p;
		}

		/// @brief Writerクラスを登録
		/// @param {id} ノード識別子
		/// @param {buffer} バッファバイト列
		/// @return Writerのインスタンスが存在しているかを取得するためのフラグ(メモリ所有権はこのクラスにあります)
		/// @remark Writerのデストラクタでポインタ先にfalseを代入してください
		template<size_t N>
		bool* joinWriter(uint16_t id, uint8_t (&buffer)[N], uint32_t& timestamp)
		{
			auto p = new bool(true);
			writers.push_back({ id, buffer, sizeof buffer, &timestamp, p });
			return p;
		}

	private:

		// @brief 受信割り込み開始
		void enableReaderIntterrupt()
		{
			pinMode(Interrupt, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(Interrupt), [] {
				can_frame msg;
				if (self->bus.readMessage(&msg) == MCP2515::ERROR_OK) {
					const auto event = [&msg](Node& reader)
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
						if (msg.can_id == it->id)
						{
							if (it->instanceAlived)
							{
								event(*it);
								++it;
							}
							else
							{
								// インスタンスが存在しない場合破棄
								delete it->instanceAlived;
								it = self->readers.erase(it);
							}
						}
					}
				}
			}, CHANGE);

		}

};

template<uint8_t Cs, uint8_t Interrupt>
CanBusSpi<Cs, Interrupt>* CanBusSpi<Cs, Interrupt>::self;