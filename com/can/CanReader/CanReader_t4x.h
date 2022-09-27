#pragma once

#include "CanBase.h"
#include "FunctionBinder.h"

template<size_t N>
class CanReader : private CanBase, private FunctionBinder<const CanBase::Message_t&>
{
		const uint8_t id;
		uint8_t buffer[N];

	public:

		/// @param id 信号識別ID
		CanReader(const uint8_t id)
			: id(id)
		{
			CanBase::begin();
		}

		constexpr const uint8_t& operator[](uint8_t index) const {
			return buffer[index];
		}
		constexpr uint8_t size() const {
			return N;
		}

		void show(const char end = {}) const {
			for (const auto& buf : buffer)
				Serial.print(buf), Serial.print('\t');
			Serial.print(end);
		}

	private:
		/// @brief 受信割り込み
		/// @param msg CAN_message_t構造体
		void callback(const Message_t& msg) override {
			
			/// format[8byte]
			/// [packetIndex][data][data][data][data][data][data][data]

			if (msg.id == id) {
				const uint8_t packetIndex = msg.buf[0];  // 分割したデータの要素番号

				for (uint8_t i = 0; i < 7; i++) {
					const uint8_t bufIndex = i + packetIndex * 7;
					if (bufIndex < N)  // 配列範囲
						buffer[bufIndex] = msg.buf[i + 1];  /*先頭1byteを飛ばす*/
					else
						break;
				}
			}
		}

};
