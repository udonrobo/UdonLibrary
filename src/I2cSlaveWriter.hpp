///   @file   I2cSlaveReader.h
///   @brief  I2cスレーブ送信用クラス
///   @author 大河 祐介
///   @date   2022/09/24


#pragma once
#include <Wire.h>

/// @param N 送信サイズ
template<size_t N>
class I2cSlaveWriter {

	public:

		/// @param address I2cアドレス
		/// @param clock   通信レート
		I2cSlaveWriter(const uint8_t address, const uint32_t clock = 400000UL) noexcept {
			Wire.begin(address);
			Wire.setClock(clock);
			Wire.onRequest([] {
				Wire.write(buffer, N);
			});
		}
		~I2cSlaveWriter() noexcept {
			Wire.end();
		}

		/// @return 設定送信サイズ
		constexpr uint8_t size() const noexcept {
			return N;
		}

		/// @brief 送信データセット
		void setArrayData(const uint8_t array[]) noexcept {
			memcpy(buffer, array, size);
		}
		void setSingleData(const uint8_t index, const uint8_t value) noexcept {
			buffer[index] = value;
		}
		void setByteData(const uint8_t index, const uint8_t value) noexcept {
			buffer[index] = value;
		}
		void setBitData(const uint8_t byteIndex, const uint8_t bitIndex, const bool value) noexcept {
			bitWrite(buffer[byteIndex], bitIndex, value);
		}
		uint8_t& operator[](const uint8_t index) const noexcept {
			return buffer[index];
		}
		void clear() noexcept {
			buffer = {};
		}

		/// @brief 送信内容を表示
		/// @param end   オプション [\n, \t ..]
		/// @param radix 基数      [BIN, HEX ..]
		void show(const char end = {}, const uint16_t radix = DEC) const noexcept {
			for (const auto& buffer : buffer)
				Serial.print(buffer, radix), Serial.print('\t');
			Serial.print(end);
		}

	private:
		static uint8_t buffer[N];  /// 送信バッファ
};

template<size_t N> uint8_t I2cSlaveWriter<N>::buffer[N];
