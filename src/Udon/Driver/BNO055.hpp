//
//    BNO055 9軸センサー
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#if defined(ARDUINO) && !defined(UDON_TEENSY_I2C_SLAVE_MODE)

#    include <Udon/Thirdparty/Adafruit_BNO055/Adafruit_BNO055.h>
#    include <Udon/Types/Euler.hpp>
#    include <Udon/Types/Quaternion.hpp>

namespace Udon
{

    /// @brief BNO055 9軸センサー
    class BNO055
        : Adafruit_BNO055
    {
        /// @brief 回転方向
        QuaternionDirection direction;

        /// @brief 内積値消去用オフセット
        Quaternion offset;

        Quaternion quaternion;

    public:
        /// @brief コンストラクタ
        /// @param wire I2C ポート
        /// @param direction 回転方向
        BNO055(TwoWire& wire, const QuaternionDirection& direction = { true, true, true })
            : Adafruit_BNO055(-1, 0x28, &wire)
            , direction(direction)
            , offset(Quaternion::Identity())
            , quaternion(Quaternion::Identity())
        {
        }

        /// @brief 初期化パラメータ
        struct Parameter
        {
            /// @brief 動作モード(デフォルト: NDOF)
            adafruit_bno055_opmode_t mode = OPERATION_MODE_NDOF;

            /// @brief 軸の割り当て設定(デフォルト: P1 = 標準)
            adafruit_bno055_axis_remap_config_t remapConfig = REMAP_CONFIG_P1;

            /// @brief 軸の符号設定(デフォルト: P1 = 標準)
            adafruit_bno055_axis_remap_sign_t remapSign = REMAP_SIGN_P1;

            /// @brief 外部クリスタルを使用するか(デフォルト: false)
            bool useExternalCrystal = false;
        };

        /// @brief 通信開始 (デフォルト設定)
        /// @return 正常に開始できたかどうか
        bool begin()
        {
            return begin(Parameter());
        }

        /// @brief 通信開始 (設定指定)
        /// @param parameter 初期化パラメータ
        /// @return 正常に開始できたかどうか
        bool begin(const Parameter& parameter)
        {
            if (!Adafruit_BNO055::begin(parameter.mode))
            {
                return false;
            }

            setAxisRemap(parameter.remapConfig);
            setAxisSign(parameter.remapSign);
            setExtCrystalUse(parameter.useExternalCrystal);

            return true;
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
        Quaternion getQuaternion() const
        {
            return (offset.inverse() * quaternion).directionRevision(direction);    // オフセットを引き、回転方向を修正
        }

        /// @brief オイラー角を取得
        /// @return オイラー角
        Euler getEuler() const
        {
            return getQuaternion().toEuler();
        }

        /// @brief ヨー角(旋回角)を取得
        /// @return ヨー角
        double getYaw() const
        {
            return getQuaternion().toYaw();
        }

        /// @brief オイラー角をシリアルポートに出力
        void show() const
        {
            Show(getEuler());
        }
    };

}    // namespace Udon

#endif
