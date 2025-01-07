//
//    ロボマスモーター
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Com/Can.hpp>
#include <Udon/Algorithm/Math.hpp>
#include <Udon/Types/Range.hpp>
#include <Udon/Types/Direction.hpp>

namespace Udon
{
    /// @brief RoboMaster 基底クラス
    /// @note このクラスは多態性のための基底クラスで、直接インスタンス化することはできません。Udon::RoboMasterC610 または Udon::RoboMasterC620 クラスを使用してください。
    class RoboMasterBase
    {
    public:
        /// @brief コンストラクタ
        /// @param bus CAN通信バス
        /// @param motorId モーターID (1~8)
        /// @param direction 回転方向
        RoboMasterBase(Udon::ICanBus& bus, int motorId, Udon::Direction direction = Udon::Direction::Forward)
            : bus{ bus }
            , motorId{ motorId }
            , direction{ direction }
        {
            // 送信者インスタンスを作成 id = 0x1FF(id1~4) or 0x200(id5~8)
            // createTx 関数はすでに存在する場合、存在するインスタンスを返す。そのため複数のロボマスインスタンスで同じ送信者インスタンスを共有できる。
            if (motorId > 4)
                nodeTx = bus.createTx(static_cast<uint32_t>(0x1FF), 8);
            else
                nodeTx = bus.createTx(static_cast<uint32_t>(0x200), 8);

            // 受信者インスタンスを作成 id = 0x200 + id
            const uint32_t rxId = static_cast<uint32_t>(0x200 + Constrain(motorId, 1, 8));
            nodeRx = bus.createRx(rxId, 8);
            nodeRx->onReceive = onReceive;
            nodeRx->param = this;
        }


        /// @brief コピーコンストラクタ
        /// @note この関数は明示的に削除されています。
        RoboMasterBase(const RoboMasterBase&) = delete;


        /// @brief ムーブコンストラクタ
        RoboMasterBase(RoboMasterBase&& other)
            : bus{ other.bus }
            , motorId{ other.motorId }
            , direction{ other.direction }
            , nodeTx{ other.nodeTx }
            , nodeRx{ other.nodeRx }
        {
            nodeRx->param = this;    // ここでotherに登録されていたthisポインタを上書きするため、コピーすることができない
        }


        /// @brief モーターの角度を取得
        /// @note 無限の範囲の角度を扱える
        /// @return 角度 [rad]
        double getAngle() const
        {
            return (angle + offsetAngle) * 2 * Udon::Pi / ppr * Udon::DirectionToSign(direction);
        }


        /// @brief モーターの角度を取得
        /// @note 無限の角度を扱えない
        /// @return 角度 [rad]
        double getRawAngle() const
        {
            return angle * 2 * Udon::Pi / ppr * Udon::DirectionToSign(direction);
        }


        /// @brief モーターの速度を取得
        /// @return 速度 [rpm]
        int16_t getVelocity() const
        {
            const auto velocity = (nodeRx->data[2] << 8 | nodeRx->data[3]) * Udon::DirectionToSign(direction);
            return static_cast<int16_t>(velocity);
        }


        /// @brief モーターのトルク電流を取得
        /// @return トルク電流 [mA]
        int16_t getTorqueCurrent() const
        {
            const int current = (nodeRx->data[4] << 8 | nodeRx->data[5]) * Udon::DirectionToSign(direction);
            return static_cast<int16_t>(current);
        }


        /// @brief モーターの温度を取得
        /// @return 温度 [℃]
        uint8_t getTemperature() const
        {
            return nodeRx->data[6];
        }

        /// @brief モーターの電流を設定
        /// @param current 電流値
        virtual void setCurrent(int16_t current) = 0;

        /// @brief 指定可能電流範囲
        /// @return 電流範囲
        virtual Udon::Range<int16_t> getCurrentRange() const = 0;

    protected:
        void setRawCurrent(int16_t rawCurrent)
        {
            const auto transmitCurrent = static_cast<int16_t>(rawCurrent * Udon::DirectionToSign(direction) * 16384 / 20000);

            if (motorId > 4)
            {
                const auto motorIndex = (motorId - 4 - 1) * 2;
                nodeTx->data[motorIndex + 0] = transmitCurrent >> 8 & 0xff;    // high byte
                nodeTx->data[motorIndex + 1] = transmitCurrent >> 0 & 0xff;    // low byte
            }
            else
            {
                const auto motorIndex = (motorId - 1) * 2;
                nodeTx->data[motorIndex + 0] = transmitCurrent >> 8 & 0xff;    // high byte
                nodeTx->data[motorIndex + 1] = transmitCurrent >> 0 & 0xff;    // low byte
            }
        }
        
    private:
        /// @brief 通信バス
        Udon::ICanBus& bus;

        /// @brief モーターID
        int motorId;

        /// @brief 回転方向
        Udon::Direction direction;

        /// @brief 送信バッファ
        Udon::CanTxNode* nodeTx;


        /// @brief 受信バッファ
        Udon::CanRxNode* nodeRx;


        /// @brief オフセット角度 (生の角度は無限の角度を扱えないので、無限の角度を扱えるようにオフセットを加減算する)
        int64_t offsetAngle = 0;
        int64_t angle = 0;


        /// @brief エンコーダー分解能
        static constexpr int32_t ppr = 8192;

        static void onReceive(void* p)
        {
            auto self = static_cast<RoboMasterBase*>(p);

            // 変化角を計算
            const auto currentAngle = self->nodeRx->data[0] << 8 | self->nodeRx->data[1];
            const auto dTheta = self->angle - currentAngle;
            self->angle = currentAngle;

            // 変化量がいきなり半周を超えた -> 計算値が-π~π間を通過 -> 一周分オフセットを加減算
            if (dTheta > ppr / 2)
            {
                self->offsetAngle += ppr;
            }
            else if (dTheta < -ppr / 2)
            {
                self->offsetAngle -= ppr;
            }
        }
    };

    /// @brief RoboMasterC610クラス
    /// @note C610 ドライバを用いてモーターを制御する
    class RoboMasterC610
        : public RoboMasterBase
    {
    public:
        /// @brief コンストラクタ
        /// @param bus CAN通信バス
        /// @param motorId モーターID (1~8)
        /// @param direction 回転方向
        using RoboMasterBase::RoboMasterBase;

        /// @brief 指定可能電流範囲
        Udon::Range<int16_t> getCurrentRange() const override
        {
            return { -10000, 10000 };
        }

        /// @brief モーターの電流を設定
        /// @param current 電流値 [-10000, 10000] (単位: mA)
        void setCurrent(int16_t current) override
        {
            RoboMasterBase::setRawCurrent(Constrain(current, getCurrentRange()));
        }
    };


    /// @brief RoboMasterC620クラス
    /// @note C620 ドライバを用いてモーターを制御する
    class RoboMasterC620
        : public RoboMasterBase
    {
    public:
        /// @brief コンストラクタ
        /// @param bus CAN通信バス
        /// @param motorId モーターID (1~8)
        /// @param direction 回転方向
        using RoboMasterBase::RoboMasterBase;

        /// @brief 指定可能電流範囲
        Udon::Range<int16_t> getCurrentRange() const override
        {
            return { -20000, 20000 };
        }

        /// @brief モーターの電流を設定
        /// @param current 電流値 [-20000, 20000] (単位: mA)
        void setCurrent(int16_t current) override
        {
            RoboMasterBase::setRawCurrent(Constrain(current, getCurrentRange()));
        }
    };
}    // namespace Udon
