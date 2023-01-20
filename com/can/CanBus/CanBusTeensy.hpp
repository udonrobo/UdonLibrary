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

#	include "list.hpp"
#	include "CanReader.hpp"
#	include "CanWriter.hpp"

/// @tparam {Bus} バス種類 (CAN0,CAN1,CAN2,CAN3)
template<CAN_DEV_TABLE Bus>
class CanBusTeensy {

		static CanBusTeensy* self;

		FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;

		IntervalTimer readerIsr;  // 送信割り込み用タイマー

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

		CanBusTeensy()
			: bus()
			, readers()
			, writers()
			, readTimestamp()
			, writeTimestamp()
		{
			self = this;
		}

		~CanBusTeensy() {
			end();
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
					for (auto && it : self->readers) {
						if (msg.id == it.id)
						{
							event(it);
						}
					}
					self->readTimestamp = micros();
				});

				// 受信割り込み開始
				readerIsr.begin(
				    [] { self->bus.events(); },
				    100
				);
			}
		}

		void end() {
			readerIsr.end();
			bus.disableFIFO();
			bus.disableFIFOInterrupt();
		}

		bool isEnableReader() const {
			return micros() - readTimestamp < 50;
		}
		bool isEnableWriter() const {
			return micros() - writeTimestamp < 50;
		}

		/// @brief バスを更新
		/// @param {writeIntervalUs} 送信間隔
		void update(uint32_t writeIntervalUs = 5000)
		{
			const auto now = micros();
			if (writers.size() && now - writeTimestamp >= writeIntervalUs)
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

				for (auto && it : self->writers) {
					event(it);
				}
				writeTimestamp = now;
			}
		}

		/// @brief Readerインスタンスをバスに追加
		/// @param r Readerインスタンス
		template<class MessageTy>
		void join(CanReader<MessageTy>& r)
		{
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

template<CAN_DEV_TABLE Bus>
CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;

#endif
