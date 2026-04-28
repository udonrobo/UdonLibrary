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
#    include <Udon/Types/Vector3D.hpp>

namespace Udon
{

    /// @brief BNO055 9軸センサー
    class BNO055
        : Adafruit_BNO055
    {
        /// @brief 回転方向修正用
        QuaternionDirection direction;

        /// @brief 姿勢オフセット
        Quaternion offset;

        /// @brief 最新の姿勢クォータニオン
        Quaternion quaternion;

        /// @brief 最新の生ジャイロ値 [deg/s]
        Vec3 gyro;

    public:
        /// @brief コンストラクタ
        /// @param wire I2C ポート
        /// @param direction 回転方向
        BNO055(TwoWire& wire, const QuaternionDirection& direction = { true, true, true })
            : Adafruit_BNO055(-1, 0x28, &wire)
            , direction(direction)
            , offset(Quaternion::Identity())
            , quaternion(Quaternion::Identity())
            , gyro()
        {
        }

        /// @brief 初期化パラメータ
        struct Parameter
        {
            /// @brief 動作モード
            adafruit_bno055_opmode_t mode = OPERATION_MODE_NDOF;

            /// @brief 軸リマップ設定
            adafruit_bno055_axis_remap_config_t remapConfig = REMAP_CONFIG_P1;

            /// @brief 軸符号設定
            adafruit_bno055_axis_remap_sign_t remapSign = REMAP_SIGN_P1;

            /// @brief 外部水晶振動子の使用有無
            bool useExternalCrystal = false;
        };

        /// @brief 初期化
        /// @return 初期化成功時 true
        bool begin()
        {
            return begin(Parameter());
        }

        /// @brief 初期化
        /// @param parameter 初期化パラメータ
        /// @return 初期化成功時 true
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

        /// @brief 動作モードを切り替える
        /// @param mode 設定するモード
        /// @note 内部で一旦 CONFIG モードに入り、25ms 程度のウェイトが発生する
        void setMode(adafruit_bno055_opmode_t mode)
        {
            Adafruit_BNO055::setMode(mode);
        }

        /// @brief 現在の動作モードを取得
        /// @return 現在の動作モード
        adafruit_bno055_opmode_t getMode()
        {
            return Adafruit_BNO055::getMode();
        }

        /// @brief 現在の姿勢を基準姿勢としてオフセットを設定
        void clear()
        {
            offset = quaternion;
        }

        /// @brief センサー値を更新
        void update()
        {
            const auto q = Adafruit_BNO055::getQuat();
            quaternion   = { q.x(), q.y(), q.z(), q.w() };

            const auto g = Adafruit_BNO055::getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
            gyro = { g.x(), g.y(), g.z() };
        }

        /// @brief 姿勢クォータニオンを取得
        /// @return オフセット適用済みクォータニオン
        Quaternion getQuaternion() const
        {
            return (offset.inverse() * quaternion).directionRevision(direction);    // オフセットを引き、回転方向を修正
        }

        /// @brief 姿勢オイラー角を取得
        /// @return オイラー角
        Euler getEuler() const
        {
            return getQuaternion().toEuler();
        }

        /// @brief ヨー角を取得
        /// @return ヨー角
        double getYaw() const
        {
            return getQuaternion().toYaw();
        }

        /// @brief 生ジャイロ値を取得 [deg/s]
        /// @return 各軸角速度
        Vec3 getGyro() const
        {
            return gyro;
        }

        /// @brief 姿勢オイラー角をシリアル出力
        void show() const
        {
            Show(getEuler());
        }
    };

}    // namespace Udon

#endif
