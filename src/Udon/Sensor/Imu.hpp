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

    class Imu
    {
        /// @brief 回転方向
        Udon::Quaternion direction;

        /// @brief 内積値消去用オフセット
        Udon::Quaternion offset;

    public:
        /// @brief コンストラクタ
        /// @param Device Deviceオブジェクト
        /// @param direction 回転方向
        Imu(const Udon::Euler3D<bool>& direction = { true, true, true })
            : direction(direction.toQuaternion())
            , offset(Udon::Quaternion::Identity())
        {
        }

        virtual Udon::Quaternion getRawQuaternion() const = 0;

        /// @brief 値を消去する
        void clear()
        {
            offset = getRawQuaternion();
        }

        /// @brief オイラー角を取得
        /// @return オイラー角
        Udon::Euler getEuler() const
        {
            return getQuaternion().toEuler();
        }

        Udon::Quaternion getQuaternion() const
        {
            return offset.inverce() * getRawQuaternion() * direction;
        }

        /// @brief オイラー角をシリアルポートに出力
        void show()
        {
            Udon::Show(getEuler());
        }
    };

}    // namespace Udon