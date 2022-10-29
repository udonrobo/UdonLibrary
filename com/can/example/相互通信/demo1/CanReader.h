/// @file   CanReader.h
/// @date   2022/09/27
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

#include "CanBase.h"
#include "FunctionBinder.h"

/// @brief モーターを使用する場合のバイト数を求める
/// @param count モーター数
/// @return バイト数
#define USE_MOTOR(count) (count + 1 + count / 8)

template<uint8_t Size>
class CanReader : private CanBase, private CanBase::FunctionBinder_t {
		const uint16_t id;
		uint8_t buffer[Size];
		uint32_t lastReceiveMs;
	public:

		/// @param id 信号識別ID ~127
		CanReader(const uint16_t id) noexcept
			: id(id)
			, buffer{}
			, lastReceiveMs()
		{
			CanBase::begin();
		}

		/// @brief data gatter
		uint8_t getByteData(const uint8_t index) const {
			return buffer[index];
		}
		uint8_t getSingleData(const uint8_t index) const {
			return buffer[index];
		}
		bool getBitData(const uint8_t byteIndex, const uint8_t bitIndex) const {
			return bitRead(buffer[byteIndex], bitIndex);
		}
		int16_t getMotorData(const uint8_t index) const {
			const uint8_t dirByteIndex = Size - 1 - index / 8;  /// 配列末端バイトから
			const uint8_t dirBitIndex  = index % 8;             /// 先頭ビットから
			const bool dir = bitRead(buffer[dirByteIndex], dirBitIndex);
			return buffer[index] * (dir ? 1 : -1);
		}

		explicit operator bool() const {
			return millis() - lastReceiveMs > 30;
		}
		constexpr const uint8_t& operator[](uint8_t index) const {
			return buffer[index];
		}
		constexpr uint8_t size() const {
			return Size;
		}

		void show(const char end = {}) const {
			for (const auto& buf : buffer)
				Serial.print(buf), Serial.print('\t');
			Serial.print(end);
		}
		void showMotor(const char end = {}) const {
			for (uint8_t i = 0; i < Size - 1 - Size / 8; i++)
				Serial.print(getMotorData(i)), Serial.print('\t');
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
