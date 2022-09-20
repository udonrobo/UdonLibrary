/// @file   Gyro.h
/// @date   2022/09/22
/// @brief  ジャイロセンサから旋回角を取得するクラス
/// @author 大河 祐介

#pragma once

#define SERIAL_INSTANCE Serial1

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
		static void begin() {
			serial.begin(115200);
			delay(100);
			update();
			initOffset = rawYaw;
		}

		/// @brief 旋回角を更新する
		/// @note  必須
		static double update() {
			if (Serial1.available() >= 3) {
				if (Serial1.read() == 'H') {
					rawYaw = (Serial1.read() << 8) | Serial1.read();
					rawYaw = (double)rawYaw / 100;
					Serial.println(rawYaw);
					if (rawYaw > 180)
						rawYaw -= 360;
					else if (rawYaw < -180)
						rawYaw += 360;
					return rawYaw;
				}
			}
			return rawYaw;
		}

		/// @brief 旋回角を0にする
		void clear() {
			offset = rawYaw;
		}

		/// 旋回角取得
		/// @return 旋回角 [-180~180deg]
		double yaw() const {
			double yaw = rawYaw - offset;
			if (yaw > 179.99)
				yaw -= 359;
			else if (yaw < -179.99)
				yaw += 359;
			return yaw;
		}
};

template<class HardwareSerial> HardwareSerial& _Gyro<HardwareSerial>::serial     = SERIAL_INSTANCE;
template<class HardwareSerial> double          _Gyro<HardwareSerial>::rawYaw     = 0;
template<class HardwareSerial> double          _Gyro<HardwareSerial>::initOffset = 0;

using Gyro = _Gyro<decltype(SERIAL_INSTANCE)>;

#undef SERIAL_INSTANCE
