/// @file   CanReader.h
/// @date   2022/09/27
/// @brief  CAN通信受信クラス
/// @format head:[id+packetId] data:[8bytes]
/// @author 大河 祐介

#pragma once

#include "CanBase.h"
#include "FunctionBinder.h"

template<size_t N>
class CanReader : private CanBase, private FunctionBinder<void(const CanBase::Message_t&)>
{
		const uint8_t id;
		uint8_t buffer[N];
		uint32_t lastReceiveMs;

	public:

		/// @param id 信号識別ID ~127
		CanReader(const uint8_t id)
			: id(id)
			, buffer{}
			, lastReceiveMs()
		{
			CanBase::begin();
		}

		explicit operator bool() const {
			return millis() - lastReceiveMs < 30;
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
		void callback(const Message_t& msg) override {
			Serial.println("call");
			if (msg.signalId == id) {
				for (uint8_t i = 0; i < 8; i++) {
					const uint8_t bufIndex = i + msg.packetId * 8;
					if (bufIndex < N)  // 配列範囲
						buffer[bufIndex] = msg.data[i];
					else
						break;
				}
				lastReceiveMs = millis();
			}
		}

};
