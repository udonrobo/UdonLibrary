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
//    BNO055 9軸センサー
//
//-------------------------------------------------------------------

#pragma once

#if defined(ARDUINO) && !defined(UDON_TEENSY_I2C_SLAVE_MODE)

#    include <Adafruit_BNO055.h>
#    include <Udon/Types/Quaternion.hpp>

namespace Udon
{

    class BNO055
        : Adafruit_BNO055
    {

        Udon::Quaternion quaternion;

    public:
        BNO055(TwoWire& wire)
            : Adafruit_BNO055(-1, 0x28, &wire)
            , quaternion()
        {
        }

        /// @brief 通信開始
        /// @remark Wire.begin() が Adafruit_BNO055ライブラリから呼び出されます
        /// @return 正常に開始できたかどうか
        bool begin()
        {
            return Adafruit_BNO055::begin();
        }

        /// @brief 更新
        void update()
        {
            const auto q = Adafruit_BNO055::getQuat();

            quaternion = Udon::Quaternion(q.x(), q.y(), q.z(), q.w());
        }

        /// @brief オイラー角取得
        /// @return オイラー角
        Udon::Euler getEuler() const
        {
            return quaternion.toEuler();
        }

        /// @brief クォータニオン取得
        /// @return クォータニオン
        Udon::Quaternion getQuaternion() const
        {
            return quaternion;
        }

        void show() const
        {
            getEuler().show();
        }
    };

}    // namespace Udon

#endif
