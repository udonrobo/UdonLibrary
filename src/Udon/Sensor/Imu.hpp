//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    9軸センサー
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Types/Euler.hpp>

namespace Udon
{

    template <class Device>
    class Imu : public Device
    {
        /// @brief 回転方向
        Udon::Quaternion direction;

        /// @brief 内積値消去用オフセット
        Udon::Quaternion home;

    public:
        /// @brief コンストラクタ
        /// @param Device Deviceオブジェクト
        /// @param direction 回転方向
        Imu(Device&& device, Udon::Euler3D<bool>&& direction = { true, true, true })
            : Device(std::move(device))
            , direction(direction.toQuaternion())
            , home()
        {
        }

        /// @brief 値を消去する
        void clear()
        {
            home = Device::getQuaternion();
        }

        /// @brief オイラー角を取得
        /// @return オイラー角
        Udon::Euler getEuler() const
        {
            return getQuaternion().getEuler();
        }

        Udon::Quaternion getQuaternion() const
        {
            return home.inverce() * Device::getQuaternion() * direction;
        }

        /// @brief オイラー角をシリアルポートに出力
        void show()
        {
            getEuler().show();
        }
    };

}    // namespace Udon