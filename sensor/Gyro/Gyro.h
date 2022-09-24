/// @file   Gyro.h
/// @date   2022/09/22
/// @brief  旋回角取得クラス
/// @author 大河 祐介

#pragma once

#define GYRO_SERIAL Serial1

template<class HardwareSerial>
class _Gyro {
		static HardwareSerial& serial; /// シリアルインスタンス
		static double rawYaw;          /// 旋回角
		static double initOffset;      /// 起動時の旋回角オフセット
		double offset;                 /// インスタンスごとの旋回角オフセット
	public:
		_Gyro() noexcept
			: offset()
		{}

		/// @brief 初期化
		/// @note  必須
		static void begin(const uint32_t baudrate = 115200) {
			serial.begin(baudrate);
			delay(100);
			update();
			initOffset = rawYaw;
		}

		/// @brief 旋回角を更新する
		/// @note  必須
		static void update() {
			while (serial.available())
			{
				if (serial.read() == 'H')  /// 復号化
				{
					const auto receiveValue = (serial.read() << 8) | (serial.read() << 0);
					rawYaw = static_cast<int16_t>(receiveValue) / 100.0 - initOffset;
				}
			}
		}

		/// @brief 旋回角を0にする
		void clear(const double offset = 0) {
			offset = rawYaw - offset;
		}

		/// 旋回角取得
		/// @return 旋回角 [-180~180deg]
		double yaw() const {
			const double yaw = rawYaw - offset;
			return fmod(fmod(yaw + 180, 360) - 360, 360) + 180;
		}

		/// @brief 表示
		void show(const char end = {}) const {
			Serial.print(yaw()), Serial.print('\t');
			Serial.print(end);
		}
};

template<class HardwareSerial> HardwareSerial& _Gyro<HardwareSerial>::serial     = GYRO_SERIAL;
template<class HardwareSerial> double          _Gyro<HardwareSerial>::rawYaw     = 0;
template<class HardwareSerial> double          _Gyro<HardwareSerial>::initOffset = 0;

using Gyro = _Gyro<decltype(GYRO_SERIAL)>;
