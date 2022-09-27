#pragma once

#include "CanBase_t4.h"

template<size_t N>
class CanMasterWriter : private CanBase {

		const uint8_t targetID;   // 送信先
		uint8_t buffer[N];        // 送信バッファ

	public:

		CanMasterWriter(const uint8_t targetID)
			: targetID(targetID)
		{
			CanBase::begin();
		}

		void update() {
			// パケットに分けて送信
			constexpr uint8_t packetNum = ceil(N / 6.0);
			for (uint8_t packetIndex = 0; packetIndex < packetNum; packetIndex++) {
				CAN_message_t msg;
				msg.id     = MASTER_ID;
				msg.buf[0] = targetID;
				msg.buf[1] = packetIndex;
				
				for (uint8_t i = 0; i < 6; i++) {
					const uint8_t bufferIndex = i + packetIndex * 6;
					if (bufferIndex < N)
						msg.buf[i + 2] = buffer[bufferIndex];
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
