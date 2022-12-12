/// @file   CanWriter.h
/// @date   2022/09/27
/// @brief  CAN通信送信クラス
/// @author 大河 祐介

#pragma once

#include "CanBase.h"

/// @brief モーターを使用する場合のバイト数を求める
/// @param count モーター数[~226]
/// @return バイト数
#define USE_MOTOR(count) (count + 1 + count / 8)

template<uint8_t Size>
class CanWriter : private CanBase {
		const uint16_t id;
		uint8_t index;
		uint8_t buffer[Size];
	public:

		/// @param id 信号識別ID ~127
		CanWriter(const uint16_t id) noexcept
			: id(id)
			, index()
			, buffer{}
		{
			CanBase::begin();
		}

		/// @brief 送信
		void update() {
			constexpr uint8_t packetSize = ceil(Size / 7.0);  /// 1パケットにデータ7byte

			/// パケットに分けて送信
			for (index = 0; index < packetSize; index++) {

				CanBase::write([](Message_t&& msg, void* p) {
					CanWriter* _this = static_cast<CanWriter*>(p);
					msg.id    = _this->id;
					msg.index = _this->index;
					for (uint8_t i = 0; i < Message_t::dataLength; i++) {
						const uint8_t bufferIndex = i + _this->index * Message_t::dataLength;
						if (bufferIndex < Size)
							msg.data[i] = _this->buffer[bufferIndex];
						else
							break;
					}
				}, this);

			}
		}

		/// @brief data setter
		void setByteData(const uint8_t index, const uint8_t value) {
			buffer[index] = value;
		}
		void setSingleData(const uint8_t index, const uint8_t value) {
			buffer[index] = value;
		}
		void setBitData(const uint8_t byteIndex, const uint8_t bitIndex, const bool value) {
			bitWrite(buffer[byteIndex], bitIndex, value);
		}
		void setMotorData(const uint8_t index, const int16_t power) {
			buffer[index] = abs(power);
			const uint8_t dirByteIndex = Size - 1 - index / 8;  /// 配列末端バイトから
			const uint8_t dirBitIndex  = index % 8;             /// 先頭ビットから
			bitWrite(buffer[dirByteIndex], dirBitIndex, power > 0);
		}

		uint8_t& operator[](const uint8_t index) {
			return buffer[index];
		}
		CanWriter& operator=(const uint8_t& value) {
			memset(buffer, value, Size);
			return *this;
		}
		constexpr uint8_t size() const {
			return Size;
		}
		constexpr uint8_t motorSize() const {
			return Size + 1 - Size / 8;
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
};
