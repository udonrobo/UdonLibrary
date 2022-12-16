/// @file   CanReader.hpp
/// @date   2022/09/27
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

#include "CanBase.hpp"
#include "BasicReader.hpp"
#include "FunctionBinder.hpp"

template<uint8_t Size>
class CanReader
	: private CanBase
	, private CanBase::FunctionBinder_t
	, public BasicReader<Size>
{
		const uint16_t id;
		uint8_t buffer[Size];
		uint32_t lastReceiveMs;
	public:

		/// @param id 信号識別ID ~127
		CanReader(const uint16_t id) noexcept
			: BasicReader<Size>(buffer)
			, id(id)
			, buffer{}
			, lastReceiveMs()
		{
			CanBase::begin();
		}

		explicit operator bool() const {
			return millis() - lastReceiveMs > 30;
		}

		void show(const char end = {}) const {
			for (const auto& buf : buffer)
				Serial.print(buf), Serial.print('\t');
			Serial.print(end);
		}
		void showMotor(const char end = {}) const {
			for (uint8_t i = 0; i < Size - 1 - Size / 8; i++)
				Serial.print(BasicReader<Size>::getMotorData(i)), Serial.print('\t');
			Serial.print(end);
		}
	private:
		/// @brief 受信割り込み
		void callback(const Message_t& msg) override {
			if (msg.id == id) {
				for (uint8_t i = 0; i < Message_t::dataLength; i++) {
					const uint8_t bufIndex = i + msg.index * Message_t::dataLength;
					if (bufIndex < Size)  // 配列範囲
						buffer[bufIndex] = msg.data[i];
					else
						break;
				}
				lastReceiveMs = millis();
			}
		}
};
