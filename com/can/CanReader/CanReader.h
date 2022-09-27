/// @file   CanReader.h
/// @date   2022/09/27
/// @brief  CAN通信受信クラス
/// @format head:[id+packetId] data:[8bytes]
/// @author 大河 祐介

#pragma once

#include "CanBase.h"
#include "FunctionBinder.h"

template<size_t N>
class CanReader : private CanBase, private FunctionBinder<const CanBase::Message_t&>
{
		const uint16_t id;
		uint8_t buffer[N];

	public:

		/// @param id 信号識別ID
		CanReader(const uint16_t id)
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
			const uint8_t packetId = msg.id >> 7 & 0b1111111;
			const uint8_t signalId = msg.id & 0b111;
			if (signalId == id) {
				for (uint8_t i = 0; i < 8; i++) {
					const uint8_t bufIndex = i + packetId * 8;
					if (bufIndex < N)  // 配列範囲
						buffer[bufIndex] = msg.buf[i];
					else
						break;
				}
			}
		}

};
