//-----------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-----------------------------------------------
//
//    9軸センサー
//
//-----------------------------------------------

#pragma once

#include <udon/types/Euler.hpp>

namespace udon
{

    template <class Device>
    class Imu
        : public Device
    {

        /// @brief 回転方向
        udon::Euler3D<bool> direction;

        /// @brief 内積値消去用オフセット
        udon::Quaternion offset;

    public:
        /// @brief コンストラクタ
        /// @param Device Deviceオブジェクト
        /// @param direction 回転方向
        Imu(
            Device&&              device,
            udon::Euler3D<bool>&& direction = { true, true, true })
            : Device(std::move(device))
            , direction(direction)
            , offset()
        {
        }

        /// @brief 値を消去する
        void clear()
        {
            offset = Device::getQuaternion();
        }

        /// @brief オイラー角を取得
        /// @return オイラー角
        udon::Euler getEuler() const
        {
            return (Device::getEuler() - offset.toEuler())
                .directionRevision(direction)       // 回転方向補正
                .normalized(-udon::Pi, udon::Pi)    // -PI~PI の間に正規化
                ;
        }

        udon::Quaternion getQuaternion() const
        {
            return Device::getQuaternion() - offset;
        }

        /// @brief オイラー角をシリアルポートに出力
        void show()
        {
            getEuler().show();
        }
    };

}    // namespace udon