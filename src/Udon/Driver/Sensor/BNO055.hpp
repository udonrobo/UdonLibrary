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
#    include <Udon/Driver/Sensor/Imu.hpp>

namespace Udon
{

    /// @brief BNO055 9軸センサー
    /// @remark 継承しているImuクラスからクオータニオン、オイラー角を取得できます
    class BNO055
        : public Imu
        , Adafruit_BNO055
    {

        Udon::Quaternion quaternion;

        /// @brief 生クォータニオン取得
        Udon::Quaternion getRawQuaternion() const override
        {
            return quaternion;
        }

    public:
        BNO055(TwoWire& wire, const Udon::Euler3D<bool>& direction = { true, true, true })
            : Imu{ direction }
            , Adafruit_BNO055(-1, 0x28, &wire)
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
            quaternion   = Udon::Quaternion(q.x(), q.y(), q.z(), q.w());
        }
    };

}    // namespace Udon

#endif
