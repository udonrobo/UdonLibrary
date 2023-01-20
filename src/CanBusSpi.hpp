/// @file   CanBusSpi.hpp
/// @date   2023/01/13
/// @brief  FlexCAN_T4ライブラリを用いたCANバス管理クラス
/// @flow   [CPU] <--SPI--> [CANコントローラ] <--CAN[TX/RX]--> [CANトランシーバ] <--CAN[H/L]--> [BUS]
/// @author 大河 祐介

#pragma once

#include <mcp2515.h>   // https://github.com/autowp/arduino-mcp2515

#include "list.hpp"
#include "CanReader.hpp"
#include "CanWriter.hpp"

template<uint8_t Cs, uint8_t Interrupt>
class CanBusSpi {

		MCP2515 bus;
		static CanBusSpi* self;

		/// @brief ノード(Reader,Writer)を管理
		struct Node {
			uint32_t id;          // ノードID
			uint8_t* buffer;      // Reader,Writerクラスのバッファに対するポインタ
			size_t size;          // バッファサイズ
			uint32_t* timestamp;  // 最後にバッファにアクセスした時刻[ms]
			void* _this;          // ノードのthisポインタ
		};

		udon::std::list<Node> readers;
		udon::std::list<Node> writers;

		uint32_t readTimestamp;
		uint32_t writeTimestamp;

	public:

		/// @brief constructor
		/// @param {spi} SPIインスタンス
		/// @param {spiClock} SPI動作クロック
		/// @remark 動作クロックは CPUクロック/2 が最大値
		CanBusSpi(SPIClass& spi, uint32_t spiClock = 10000000)
			: bus(Cs, spiClock, &spi)
			, readers()
			, writers()
			, readTimestamp()
			, writeTimestamp()
		{
			self = this;
		}

		~CanBusSpi()
		{}

		/// @brief 通信を開始
		/// @param {baudrate} CAN通信レート
		/// @param {canClock} CAN通信クロック
		void begin(CAN_SPEED baudrate = CAN_1000KBPS, CAN_CLOCK canClock = MCP_20MHZ) {
			if (readers.size() || writers.size())
			{
				bus.reset();
				bus.setBitrate(baudrate, canClock);
				bus.setNormalMode();
			}
			if (readers.size())
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
						for (auto && it : self->readers) {
							if (msg.can_id == it.id)
							{
								event(it);
							}
						}
						self->readTimestamp = micros();
					}
				}, CHANGE);
			}
		}

		bool isEnableReader() const {
			return micros() - readTimestamp < 50;
		}
		bool isEnableWriter() const {
			return micros() - writeTimestamp < 50;
		}

		/// @brief バスを更新
		/// @param {writeIntervalUs} 送信間隔
		void update(uint32_t writeIntervalUs = 5000) {
			const auto now = micros();
			if (writers.size() && now - writeTimestamp >= writeIntervalUs)
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

				for (auto && it : self->writers) {
					event(it);
				}
				writeTimestamp = now;
			}
		}

		/// @brief Readerインスタンスをバスに追加
		/// @param r Readerインスタンス
		template<class MessageTy>
		void join(CanReader<MessageTy>& r) {
			for (auto && it : readers)
			{
				if (&r == it._this)
				{
					return;
				}
			}
			auto&& hReader = r.getHandler();
			readers.push_back({
				static_cast<uint16_t>(hReader.id),
				hReader.buffer,
				hReader.size,
				hReader.timestamp,
				hReader._this
			});
		}

		/// @brief Readerインスタンスをバスから開放
		/// @param r Readerインスタンス
		template<class MessageTy>
		void detach(const CanReader<MessageTy>& r)
		{
			for (auto && it = readers.begin(); it != readers.end(); ++it)
			{
				if (it->_this == &r)
				{
					readers.erase(it);
					break;
				}
			}
		}

		/// @brief Writerインスタンスをバスに追加
		/// @param r Writerインスタンス
		template<class MessageTy>
		void join(CanWriter<MessageTy>& r)
		{
			for (auto && it : writers)
			{
				if (&r == it._this)
				{
					return;
				}
			}
			auto&& hWriter = r.getHandler();
			writers.push_back({
				static_cast<uint16_t>(hWriter.id),
				hWriter.buffer,
				hWriter.size,
				hWriter.timestamp,
				hWriter._this
			});
		}

		/// @brief Writerインスタンスをバスから開放
		/// @param r Writerインスタンス
		template<class MessageTy>
		void detach(const CanWriter<MessageTy>& r)
		{
			for (auto && it = writers.begin(); it != writers.end(); ++it)
			{
				if (it->_this == &r)
				{
					writers.erase(it);
					break;
				}
			}
		}

};

template<uint8_t Cs, uint8_t Interrupt>
CanBusSpi<Cs, Interrupt>* CanBusSpi<Cs, Interrupt>::self;
