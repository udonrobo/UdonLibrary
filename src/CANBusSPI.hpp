/// @file   CANBusSPI.hpp
/// @date   2023/01/13
/// @brief  arduino-mcp2515ライブラリを用いたCANバス管理クラス
/// @flow   [CPU] <--SPI--> [CANコントローラ] <--CAN[TX/RX]--> [CANトランシーバ] <--CAN[H/L]--> [BUS]
/// @author 大河 祐介

#pragma once

#include <mcp2515.h>   // https://github.com/autowp/arduino-mcp2515

#	include "list.hpp"  // udon::std::list

#	include "CANInfo.hpp"
#	include "CANBusInterface.hpp"

template<uint8_t CS, uint8_t Interrupt>
class CANBusSPI : public CANBusInterface
{

		MCP2515 bus;

		using DataLine = udon::std::list<CANNodeInfo*>;
		DataLine        tx   ;
		DataLine        rx   ;
		CANBusErrorInfo error;

		static CANBusSPI* self;

	public:

		/// @brief constructor
		/// @param {spi} SPIインスタンス
		/// @param {spiClock} SPI動作クロック
		/// @remark 動作クロックは CPUクロック/2 が最大値
		CANBusSPI(SPIClass& spi, uint32_t spiClock = 10000000)
			: bus(CS, spiClock, &spi)
			, tx()
			, rx()
		{
			self = this;
		}

		~CANBusSPI()
		{}

		/// @brief 通信を開始
		/// @param {baudrate} CAN通信レート
		/// @param {canClock} CAN通信クロック
		void begin(CAN_CLOCK canClock = MCP_20MHZ, CAN_SPEED baudrate = CAN_1000KBPS)
		{
			if (rx.size() || tx.size())
			{
				bus.reset();
				bus.setBitrate(baudrate, canClock);
				bus.setNormalMode();
			}
			if (rx.size())
			{
				pinMode(Interrupt, INPUT_PULLUP);
				attachInterrupt(digitalPinToInterrupt(Interrupt), [] {
					can_frame msg;
					if (self->bus.readMessage(&msg) == MCP2515::ERROR_OK) {
						const auto event = [&msg](CANNodeInfo* node)
						{
							// 先頭1バイト : パケット番号
							const uint8_t index = msg.data[0];

							// 8バイト受信データをバイト列にデコード
							for (uint8_t i = 0; i < 7; i++)
							{
								const uint8_t bufIndex = i + index * 7;
								if (bufIndex < node->length)
									node->buffer[bufIndex] = msg.data[i + 1];
								else
									break;
							}
							node->timestampUs = micros();
						};
						for (auto && it : self->rx) {
							if (msg.can_id == it->id)
							{
								event(it);
							}
						}
						self->error.timestampUs = micros();
					}
				}, CHANGE);
			}
		}

		/// @brief バスを更新
		/// @param {writeIntervalUs} 送信間隔
		void update(uint32_t writeIntervalUs = 5000)
		{
			if (tx.size() && micros() - error.timestampUs >= writeIntervalUs)
			{
				const auto event = [](CANNodeInfo* node) {
					// 一度に8バイトしか送れないため、パケットに分割し送信
					for (size_t index = 0; index < ceil(node->length / 7.0); index++)
					{
						can_frame msg;

						// 先頭1バイト : パケット番号
						msg.can_id = node->id;
						msg.data[0] = index;

						// バイト列を8バイト受信データにエンコード
						for (uint8_t i = 0; i < 7; i++)
						{
							const uint8_t bufIndex = i + index * 7;

							if (bufIndex < node->length)
								msg.data[i + 1] = node->buffer[bufIndex];
							else
								break;
						}
						// バスに送信
						self->bus.sendMessage(&msg);
					}
					node->timestampUs = micros();
				};

				for (auto && it : self->tx) {
					event(it);
				}
				error.timestampUs = micros();
			}
		}

		CANBusErrorInfo getErrorInfo() const {
			return error;
		}

		void joinTX(CANNodeInfo& node) override
		{
			join(tx, node);
		}
		void joinRX(CANNodeInfo& node) override
		{
			join(rx, node);
		}

		void detachRX(CANNodeInfo& node) override
		{
			detach(rx, node);
		}
		void detachTX(CANNodeInfo& node) override
		{
			detach(tx, node);
		}

	private:

		void join(DataLine& line, CANNodeInfo& node)
		{
			for (auto && it : line)
			{
				if (it == &node)  // インスタンスの重複を除外する
				{
					return;
				}
			}
			line.push_back(&node);
		}

		void detach(DataLine& line, CANNodeInfo& node)
		{
			for (auto && it = line.begin(); it != line.end(); ++it)
			{
				if (*it == &node)
				{
					line.erase(it);
					break;
				}
			}
		}

};

template<uint8_t CS, uint8_t Interrupt>
CANBusSPI<CS, Interrupt>* CANBusSPI<CS, Interrupt>::self;
