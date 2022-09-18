#pragma once

template<class T = double>
class _Gyro
{
		static Stream& serial;
		static T rawYaw;   /// 生の旋回角
		T offset;          /// インスタンスごとの旋回角
	public:
		_Gyro()
			: offset()
		{}

		static void init()
		{
			serial.begin(115200);
			update();
		}
		static void update()
		{
			if (serial.available() >= 3 && serial.read() == 'H')
				rawYaw = ((serial.read() << 8) | (serial.read() << 0)) / 100.0;
		}

		void clear()
		{
			offset = rawYaw;
		}

		T yaw() const
		{
			T yaw = rawYaw - offset;
			yaw += 180;
			yaw = fmod(yaw, 360);  /// 正の方向
			yaw -= 360;
			yaw = fmod(yaw, 360);  /// 負の方向
			yaw += 180;
			return yaw;
		}
};

template<class T> Stream& _Gyro<T>::serial = Serial1;
template<class T> T       _Gyro<T>::rawYaw = 0;

using Gyro = _Gyro<double>;
