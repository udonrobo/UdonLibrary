#include "CanBase.h"

#pragma once

template<size_t N>
class CanWriter : private CanBase {
	
		const uint8_t id;    // 送信先
		uint8_t buffer[N];         // 送信バッファ
		Can& can = CanBase::canReference();

	public:

		/// @brief
		/// @param id 自身のID
		CanWriter(const uint8_t id)
			: id(id) {
			CanBase::begin();
		}

		void update() {
			// パケットに分けて送信
			constexpr uint8_t packetNum = ceil(N / 7.0);
			for (uint8_t packetIndex = 0; packetIndex < packetNum; packetIndex++) {
				CAN_message_t msg;
				msg.id     = id;
				msg.buf[0] = packetIndex;
				
				for (uint8_t i = 0; i < 7; i++) {
					const uint8_t bufferIndex = i + packetIndex * 7;
					if (bufferIndex < N)
						msg.buf[i + 1] = buffer[bufferIndex];
					else
						break;
				}
				while (!can.write(msg));  // 送信
			}
		}

		constexpr uint8_t& operator[](uint8_t index) {
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

};
