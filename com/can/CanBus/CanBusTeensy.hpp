/// @file   CanBusTeensy.hpp
/// @date   2023/01/13
/// @brief  FlexCAN_T4ライブラリを用いたCANバス管理クラス
/// @flow   [CPU] <--CAN[TX/RX]--> [CANトランシーバ] <--CAN[H/L]--> [BUS]
/// @author 大河 祐介

#pragma once

//     vvv teensy3.2 vvv        vvv teensy3.5 vvv         vvv teensy3.6 vvv       vvv teensy4.0/4.1 vvv
#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)


#	include <FlexCAN_T4.h>      // https://github.com/tonton81/FlexCAN_T4.git
#	include <IntervalTimer.h>   // https://github.com/loglow/IntervalTimer.git

#	include "list.hpp"  // udon::std::list

#	include "CanHandle.hpp"
#	include "CanBusInterface.hpp"


/// @tparam {Bus} バス種類 (CAN0,CAN1,CAN2,CAN3)
template<CAN_DEV_TABLE Bus>
class CanBusTeensy : public CanBusInterface
{

		static CanBusTeensy* self;

		FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;
		IntervalTimer                             isr;

		using DataLine = udon::std::list<CanNodeHandle*>;
		DataLine          tx    ;
		DataLine          rx    ;
		CanBusErrorHandle hError;

	public:

		CanBusTeensy()
			: bus()
			, tx()
			, rx()
			, hError()
		{
			self = this;
		}

		~CanBusTeensy()
		{
			end();
		}

		/// @brief 通信を開始
		/// @param {baudrate} 通信レート
		void begin(const uint32_t baudrate = 1000000)
		{
			if (rx.size() || tx.size())
			{
				bus.begin();
				bus.setBaudRate(baudrate);
				bus.enableFIFO();
				bus.enableFIFOInterrupt();
			}
			if (rx.size())
			{
				bus.onReceive([](const CAN_message_t& msg) {
					const auto now = micros();
					const auto event = [&msg, &now](CanNodeHandle * hNode)
					{
						// 先頭1バイト : パケット番号
						const uint8_t index = msg.buf[0];

						// 8バイト受信データをバイト列にデコード
						for (uint8_t i = 0; i < 7; i++)
						{
							const uint8_t bufIndex = i + index * 7;
							if (bufIndex < hNode->length)
								hNode->buffer[bufIndex] = msg.buf[i + 1];
							else
								break;
						}
						hNode->timestampUs = now;
					};
					for (auto && it : self->rx) {
						if (msg.id == it->id)
						{
							event(it);
						}
					}

					CAN_error_t error;
					self->bus.error(error, false);
					self->hError = {
						error.TX_ERR_COUNTER,
						error.RX_ERR_COUNTER,
						now
					};
					
				});

				// 受信割り込み開始
				isr.begin(
				    [] { self->bus.events(); },
				    100
				);
			}
		}

		void end()
		{
			isr.end();
			bus.disableFIFO();
			bus.disableFIFOInterrupt();
		}

		/// @brief バスを更新
		/// @param {writeIntervalUs} 送信間隔
		void update(uint32_t writeIntervalUs = 5000)
		{
			const auto now = micros();
			if (tx.size() && now - hError.timestampUs >= writeIntervalUs)
			{
				const auto event = [&now](CanNodeHandle * hNode) {
					// 一度に8バイトしか送れないため、パケットに分割し送信
					for (size_t index = 0; index < ceil(hNode->length / 7.0); index++)
					{
						CAN_message_t msg;

						// 先頭1バイト : パケット番号
						msg.id = hNode->id;
						msg.buf[0] = index;

						// バイト列を8バイト受信データにエンコード
						for (uint8_t i = 0; i < 7; i++)
						{
							const uint8_t bufIndex = i + index * 7;

							if (bufIndex < hNode->length)
								msg.buf[i + 1] = hNode->buffer[bufIndex];
							else
								break;
						}
						// バスに送信
						while (not self->bus.write(msg));
					}
					hNode->timestampUs = now;
				};

				for (auto && it : self->tx) {
					event(it);
				}

				CAN_error_t error;
				bus.error(error, false);
				hError = {
					error.TX_ERR_COUNTER,
					error.RX_ERR_COUNTER,
					now
				};

			}
		}

		CanBusErrorHandle getErrorHandle() const {
			return hError;
		}

		void joinTX(CanNodeHandle& hNode) override
		{
			join(tx, hNode);
		}
		void joinRX(CanNodeHandle& hNode) override
		{
			join(rx, hNode);
		}

		void detachRX(CanNodeHandle& hNode) override
		{
			detach(rx, hNode);
		}
		void detachTX(CanNodeHandle& hNode) override
		{
			detach(tx, hNode);
		}

	private:

		void join(DataLine& line, CanNodeHandle& hNode)
		{
			for (auto && it : line)
			{
				if (it == &hNode)  // インスタンスの重複を除外する
				{
					return;
				}
			}
			line.push_back(&hNode);
		}

		void detach(DataLine& line, CanNodeHandle& hNode)
		{
			for (auto && it = line.begin(); it != line.end(); ++it)
			{
				if (*it == &hNode)
				{
					line.erase(it);
					break;
				}
			}
		}

};

template<CAN_DEV_TABLE Bus>
CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;

#endif
