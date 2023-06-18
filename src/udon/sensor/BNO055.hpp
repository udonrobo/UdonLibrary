#pragma once

#if defined(ARDUINO) && !defined(UDON_TEENSY_I2C_SLAVE_MODE)

#    include <Adafruit_BNO055.h>
#    include <udon/types/Quaternion.hpp>

namespace udon
{

    class BNO055
        : Adafruit_BNO055
    {

        udon::Quaternion quaternion;

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
            quaternion   = udon::Quaternion(q.x(), q.y(), q.z(), q.w());
        }

        /// @brief オイラー角取得
        /// @return オイラー角
        udon::Euler getEuler() const
        {
            return quaternion.toEuler();
        }

        /// @brief クォータニオン取得
        /// @return クォータニオン
        udon::Quaternion getQuaternion() const
        {
            return quaternion;
        }

        void show() const
        {
            getEuler().show();
        }
    };

}    // namespace udon

#endif
