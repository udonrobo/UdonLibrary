/// @file   CanWriter.h
/// @date   2022/09/27
/// @brief  CAN通信送信クラス
/// @format head:[id+packetId] data:[8bytes]
/// @author 大河 祐介

#include "CanBase.h"

#pragma once

template<size_t N>
class CanWriter : private CanBase {

		const uint8_t id;
		uint8_t buffer[N];

	public:

		/// @param id 信号識別ID ~127
		CanWriter(const uint8_t id)
			: id(id)
		{
			CanBase::begin();
		}

		void update() {
			constexpr uint8_t packetNum = ceil(N / 7.0);
			// パケットに分けて送信
			for (uint8_t packetId = 0; packetId < packetNum; packetId++) {
				Message_t msg = { static_cast<uint32_t>(packetId) << 7 | id };
				for (uint8_t i = 0; i < 8; i++) {
					const uint8_t bufferIndex = i + packetId * 8;
					if (bufferIndex < N)
						msg.buf[i] = buffer[bufferIndex];
					else
						break;
				}
				CanBase::write(msg);
			}
		}

		uint8_t& operator[](uint8_t index) {
			return buffer[index];
		}
		CanWriter& operator=(const uint8_t& r) {
			memset(buffer, r, N);
			return *this;
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
