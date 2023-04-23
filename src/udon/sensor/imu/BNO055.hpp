#pragma once

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// https://github.com/adafruit/Adafruit_Sensor.git
// https://github.com/adafruit/Adafruit_BNO055.git
// https://github.com/adafruit/Adafruit_BusIO.git

#include <udon/types/Euler.hpp>

namespace udon
{

	class BNO055
		: Adafruit_BNO055
	{

		udon::Euler euler;

	public:

		BNO055(TwoWire& wire)
			: Adafruit_BNO055(-1, 0x28, &wire)
			, euler()
		{
		}

		/// @brief 通信開始
		/// @remark Wire.begin()がAdafruit_BNO055ライブラリ側で呼び出されます
		/// @return 正常に開始できたかどうか
		bool begin()
		{
			if (!Adafruit_BNO055::begin())
			{
				return false;
			}
			return true;
		}

		/// @brief 更新
		void update()
		{
			const auto libEuler = Adafruit_BNO055::getVector(Adafruit_BNO055::VECTOR_EULER);
			euler = udon::Euler {
				udon::ToRadians(libEuler.y()),  // roll
				udon::ToRadians(libEuler.z()),  // pitch
				udon::ToRadians(libEuler.x()),  // yaw
			};
		}

		/// @brief オイラー角取得
		/// @return オイラー角
		udon::Euler getEuler() const
		{
			return euler;
		}

		void show() const
		{
			getEuler().show();
		}
	};

}  // namespace udon