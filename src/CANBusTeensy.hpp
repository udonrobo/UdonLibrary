/// @file   CANBusTeensy.hpp
/// @date   2023/01/13
/// @brief  FlexCAN_T4ライブラリを用いたCANバス管理クラス
/// @remark [CPU] <--CAN[TX/RX]--> [CANトランシーバ] <--CAN[H/L]--> [BUS]
/// @author 大河 祐介

#pragma once

//     vvv teensy3.2 vvv        vvv teensy3.5 vvv         vvv teensy3.6 vvv       vvv teensy4.0/4.1 vvv
#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)


#	include <FlexCAN_T4.h>      // https://github.com/tonton81/FlexCAN_T4.git
#	include <IntervalTimer.h>   // https://github.com/loglow/IntervalTimer.git

#	include "list.hpp"  // udon::std::list

#	include "CANInfo.hpp"
#	include "CANBusInterface.hpp"


/// @tparam {Bus} バス種類 (CAN0,CAN1,CAN2,CAN3)
template<CAN_DEV_TABLE Bus>
class CANBusTeensy : public CANBusInterface
{

		FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;
		IntervalTimer                             isr;

		using DataLine = udon::std::list<CANNodeInfo*>;
		DataLine        tx   ;
		DataLine        rx   ;
		CANBusErrorInfo error;

		static CANBusTeensy* self;

	public:

		CANBusTeensy()
			: bus   {}
			, tx    {}
			, rx    {}
			, error {}
		{
			self = this;
		}

		~CANBusTeensy()
		{
			end();
		}

		/// @brief 通信開始
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
				bus.onReceive(eventRX);
				isr.begin(
				    [] { self->bus.events(); },
				    100
				);
			}
		}

		/// @brief 通信終了
		void end()
		{
			isr.end();
			bus.disableFIFO();
			bus.disableFIFOInterrupt();
		}

		/// @brief バス更新
		/// @param {writeIntervalUs} 送信間隔
		void update(uint32_t writeIntervalUs = 5000)
		{
			if (tx.size() && micros() - error.timestampUs >= writeIntervalUs)
			{
				eventTX();
			}
		}

		/// @brief エラー情報取得
		/// @return CANBusErrorInfo構造体インスタンス
		CANBusErrorInfo getErrorInfo() const
		{
			return error;
		}

		/// @brief TXノードをバスに参加
		/// @param node
		void joinTX(CANNodeInfo& node) override
		{
			join(tx, node);
		}

		/// @brief RXノードをバスに参加
		/// @param node
		void joinRX(CANNodeInfo& node) override
		{
			join(rx, node);
		}

		/// @brief RXノードをバスから解放
		/// @param node
		void detachRX(CANNodeInfo& node) override
		{
			detach(rx, node);
		}

		/// @brief TXノードをバスから解放
		/// @param node
		void detachTX(CANNodeInfo& node) override
		{
			detach(tx, node);
		}

	private:

		/// @brief ライブラリクラスからバスのエラー情報を取得
		static CANBusErrorInfo getError()
		{
			CAN_error_t e;
			self->bus.error(e, false);
			return {
				e.TX_ERR_COUNTER,
				e.RX_ERR_COUNTER,
				micros()
			};
		}

		static void eventRX(const CAN_message_t& msg)
		{
			const auto event = [&msg](CANNodeInfo * node)
			{
				const uint8_t index = msg.buf[0];   // 先頭1バイト : パケット番号
				for (uint8_t i = 0; i < 7; i++)     // 8バイト受信データをバイト列にデコード
				{
					const uint8_t bufIndex = i + index * 7;
					if (bufIndex < node->length)
						node->buffer[bufIndex] = msg.buf[i + 1];
					else
						break;
				}
				node->timestampUs = micros();
			};
			for (auto && it : self->rx)
			{
				if (msg.id == it->id)
				{
					event(it);
				}
			}
			// self->error = getError();
		}

		static void eventTX()
		{
			const auto event = [](CANNodeInfo * node)
			{
				// 一度に8バイトしか送れないため、パケットに分割し送信
				for (size_t index = 0; index < ceil(node->length / 7.0); index++)
				{
					CAN_message_t msg;
					msg.id = node->id;
					msg.buf[0] = index;  // 先頭1バイト : パケット番号
					for (uint8_t i = 0; i < 7; i++)  // バイト列を8バイト受信データにエンコード
					{
						const uint8_t bufIndex = i + index * 7;
						if (bufIndex < node->length)
							msg.buf[i + 1] = node->buffer[bufIndex];
						else
							break;
					}
					while (not self->bus.write(msg));  // 送信
				}
				node->timestampUs = micros();
			};
			for (auto && it : self->tx) {
				event(it);
			}
			// self->error = getError();
		}

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

template<CAN_DEV_TABLE Bus>
CANBusTeensy<Bus>* CANBusTeensy<Bus>::self;

#endif
