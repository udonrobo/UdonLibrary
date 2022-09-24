///   @file   I2cSlaveReader.h
///   @brief  I2cスレーブ受信用クラス
///   @author 大河 祐介
///   @date   2022/09/24

#pragma once
#include <Wire.h>

/// @param N 受信サイズ
template<size_t N>
class I2cSlaveReader {

	public:

		/// @param address I2cアドレス
		/// @param clock   通信レート
		I2cSlaveReader(const uint8_t address, const uint32_t clock = 400000UL) noexcept
			: address(address), clock(clock)
		{
			Wire.begin(address);
			Wire.setClock(clock);
			Wire.onReceive([](int) {
				while (Wire.available()) {
					for (auto& buf : buffer)
						buf = Wire.read();
					prevReceiveMs = millis();
				}
			});
		}
		I2cSlaveReader() = default;
		~I2cSlaveReader() noexcept {
			Wire.end();
		}

		/// @return 設定受信サイズ
		constexpr uint8_t size() const noexcept {
			return N;
		}

		/// @brief 通信遮断時に復帰させる
		/// @return 通信しているか
		bool update(const uint32_t timeOutMs = 50) const noexcept {
			if (not isConnected(timeOutMs)) {
				Wire.end();
				Wire.begin(address);
				Wire.setClock(clock);
				return false;
			}
			return true;
		}

		/// @brief 通信状況を判定
		/// @param timeOutMs [ms] タイムアウト時間を指定
		/// @return 通信しているか
		bool isConnected(const uint32_t timeOutMs = 50) const noexcept {
			return (millis() - prevReceiveMs) < timeOutMs;
		}
		explicit operator bool() const noexcept {
			return isConnected();
		}

		/// @brief 受信データを返す
		uint8_t getSingleData(const uint8_t index) const noexcept {
			return buffer[index];
		}
		uint8_t getByteData(const uint8_t index) const noexcept {
			return buffer[index];
		}
		bool getBitData(const uint8_t byteIndex, const uint8_t bitIndex) const noexcept {
			return bitRead(buffer[byteIndex], bitIndex);
		}
		int16_t getMotorData(const uint8_t index) const noexcept {
			const int16_t dir = bitRead(buffer[N - 1], index) ? 1 : -1;
			return buffer[index] * dir;
		}
		const uint8_t& operator[](const uint8_t index) const noexcept {
			return buffer[index];
		}

		/// @brief 受信内容を表示
		/// @param end   オプション [\n, \t ..]
		/// @param radix 基数      [BIN, HEX ..]
		void show(const char end = {}, const uint16_t radix = DEC) const {
			for (const auto& buf : buffer)
				Serial.print(buf, radix), Serial.print('\t');
			Serial.print(end);
		}
		void showMotor(const char end = {}) const {
			for (uint8_t i = 0; i < N - 1; i++)
				Serial.print(getMotorData(i)), Serial.print('\t');
			Serial.print(end);
		}

	private:
		const  uint8_t  address;
		const  uint32_t clock;
		static uint8_t  buffer[N]    ;  /// 受信バッファ
		static uint32_t prevReceiveMs;  /// 前回受信時刻
};

template<size_t N>uint8_t  I2cSlaveReader<N>::buffer[N];
template<size_t N>uint32_t I2cSlaveReader<N>::prevReceiveMs;
