#pragma once

// IMUデバイス
#include <udon/sensor/imu/BNO055.hpp>

#include <udon/types/Euler.hpp>

namespace udon
{

	template<class Device>
	class Imu
		: public Device
	{

		/// @brief 回転方向
		udon::Euler3D<bool> direction;

		/// @brief 内積値消去用オフセット
		udon::Euler offset;

	public:

		/// @brief コンストラクタ
		/// @param Device Deviceオブジェクト
		/// @param direction 回転方向
		Imu(
			Device&& device,
			udon::Euler3D<bool>&& direction = { true, true, true }
		)
			: Device(std::move(device))
			, direction(direction)
			, offset()
		{}

		/// @brief 値を消去する
		void clear()
		{
			offset = Device::getEuler();
		}

		/// @brief オイラー角を取得
		/// @return オイラー角
		udon::Euler getEuler() const
		{
			return (Device::getEuler() - offset)
					.directionRevision(direction)  // 回転方向補正
					.normalized(-PI, PI);          // -PI~PI の間に正規化
		}

		/// @brief オイラー角をシリアルポートに出力
		void show()
		{
			getEuler().show();
		}

	};

}  // namespace udon