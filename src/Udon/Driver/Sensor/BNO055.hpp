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
#    include <Udon/Types/Vector3D.hpp>
#    include <Udon/Types/Euler.hpp>
#    include <Udon/Types/Quaternion.hpp>

namespace Udon
{

    /// @brief BNO055 9軸センサー
    class BNO055
        : Adafruit_BNO055
    {
        /// @brief 回転方向
        Udon::Vector3D<bool> direction;

        /// @brief 内積値消去用オフセット
        Udon::Quaternion offset;

        Udon::Quaternion quaternion;

    public:
        /// @brief コンストラクタ
        /// @param Device Deviceオブジェクト
        /// @param direction 回転方向
        BNO055(TwoWire& wire, Udon::Vector3D<bool>&& direction = { true, true, true })
            : Adafruit_BNO055(-1, 0x28, &wire)
            , direction(direction)
            , offset(Udon::Quaternion::Identity())
            , quaternion(Udon::Quaternion::Identity())
        {
        }

        /// @brief 通信開始
        /// @remark Wire.begin() が Adafruit_BNO055ライブラリから呼び出されます
        /// @return 正常に開始できたかどうか
        bool begin()
        {
            const auto result = Adafruit_BNO055::begin();
            // update();
            // delay(500);
            // clear();
            return result;
        }

        /// @brief 値を消去する
        void clear()
        {
            offset = quaternion;
        }

        /// @brief 更新
        void update()
        {
            const auto q = Adafruit_BNO055::getQuat();
            quaternion   = { q.x(), q.y(), q.z(), q.w() };
        }

        /// @brief クォータニオン角を取得
        /// @return クォータニオン角
        Udon::Quaternion getQuaternion() const
        {
            const auto q = offset.inverce() * quaternion;
            return {
                q.x * (direction.x ? 1 : -1),
                q.y * (direction.y ? 1 : -1),
                q.z * (direction.z ? 1 : -1),
                q.w
            };
        }

        /// @brief オイラー角を取得
        /// @return オイラー角
        Udon::Euler getEuler() const
        {
            return getQuaternion().toEuler();
        }

        /// @brief オイラー角をシリアルポートに出力
        void show() const
        {
            Udon::Show(getEuler());
        }
    };

}    // namespace Udon

#endif
