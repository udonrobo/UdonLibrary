//
//    ロボマスモーター
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Com/Can.hpp>
#include <Udon/Algorithm/Math.hpp>

namespace Udon
{

    namespace Impl
    {

        /// @brief RoboMaster 基底クラス
        /// @note モータードライバの仕様の違いによって派生クラスを作成する
        class RoboMasterBase
        {
        public:
            /// @brief コンストラクタ
            /// @param bus CAN通信バス
            /// @param motorId モーターID (1~8)
            /// @param direction 回転方向
            RoboMasterBase(Udon::ICanBus& bus, uint8_t motorId, bool direction = true)
                : bus{ bus }
                , motorId{ motorId }
                , direction{ direction }
                , nodeTx{ bus.createTx(static_cast<uint32_t>(0x200), 8) }
                , nodeRx{ bus.createRx(static_cast<uint32_t>(0x200 + Constrain(motorId, 1, 8)), 8) }
            {
                nodeRx->onReceive = onReceive;
                nodeRx->param = this;
            }

            /// @brief 受信idは0x201から一つのコントローラにつき一つのidを持つ
            /// @param 送信idは0x200にモータ4つのデータを送信する
            RoboMasterBase(const RoboMasterBase& other)
                : bus{ other.bus }
                , motorId{ other.motorId }
                , direction{ other.direction }
                , nodeTx{ other.nodeTx }
                , nodeRx{ other.nodeRx }
            {
                nodeRx->param = this;
            }


            /// @brief モーターの角度を取得
            /// @note 無限の範囲の角度を扱える (生の角度は8192で一周)
            /// @return 角度 [rad]
            double getAngle() const
            {
                return (angle + offsetAngle) * 2 * Udon::Pi / ppr * directionSign();
            }


            /// @brief モーターの速度を取得
            /// @return 速度 [rpm]
            int16_t getVelocity() const
            {
                return (nodeRx->data[2] << 8 | nodeRx->data[3]) * directionSign();
            }


            /// @brief モーターのトルク電流を取得
            /// @return トルク電流 [mA]
            int16_t getTorqueCurrent() const
            {
                const int current = nodeRx->data[4] << 8 | nodeRx->data[5];
                return current * directionSign();
            }


            /// @brief モーターの温度を取得
            /// @return 温度 [℃]
            uint8_t getTemperature() const
            {
                return nodeRx->data[6];
            }

        protected:
            /// @brief モーターの電流を設定
            /// @param current 電流値
            void setCurrent(int16_t current)
            {
                current = current * directionSign();

                const auto transmitCurrent = static_cast<int16_t>(current * 16384 / 20000);    // 16bit符号なし整数に変換
                const auto motorIndex = (motorId - 1) * 2;

                nodeTx->data[motorIndex + 0] = transmitCurrent >> 8 & 0xff;    // high byte
                nodeTx->data[motorIndex + 1] = transmitCurrent >> 0 & 0xff;    // low byte
            }

        private:
            /// @brief 通信バス
            Udon::ICanBus& bus;

            /// @brief モーターID
            uint8_t motorId;

            /// @brief 回転方向
            bool direction;

            /// @brief 送信バッファ
            Udon::CanTxNode* nodeTx;

            /// @brief 受信バッファ
            Udon::CanRxNode* nodeRx;


            /// @brief オフセット角度 (生の角度は無限の角度を扱えないので、無限の角度を扱えるようにオフセットを加減算する)
            int64_t offsetAngle = 0;
            int64_t angle = 0;


            /// @brief エンコーダー分解能
            static constexpr int32_t ppr = 8192;

            int directionSign() const
            {
                return direction ? 1 : -1;
            }

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

    }    // namespace Impl


    /// @brief RoboMasterC610クラス
    /// @note C610 ドライバを用いてモーターを制御する
    class RoboMasterC610
        : public Impl::RoboMasterBase
    {
    public:
        /// @brief コンストラクタ
        /// @param bus CAN通信バス
        /// @param motorId モーターID (1~8)
        /// @param direction 回転方向
        using RoboMasterBase::RoboMasterBase;

        /// @brief モーターの電流を設定
        /// @param current 電流値 [-10000, 10000] (単位: mA)
        void setCurrent(int16_t current)
        {
            RoboMasterBase::setCurrent(Constrain(current, -10000, 10000));
        }
    };


    /// @brief RoboMasterC620クラス
    /// @note C620 ドライバを用いてモーターを制御する
    class RoboMasterC620
        : public Impl::RoboMasterBase
    {
    public:
        /// @brief コンストラクタ
        /// @param bus CAN通信バス
        /// @param motorId モーターID (1~8)
        /// @param direction 回転方向
        using RoboMasterBase::RoboMasterBase;

        /// @brief モーターの電流を設定
        /// @param current 電流値 [-20000, 20000] (単位: mA)
        void setCurrent(int16_t current)
        {
            RoboMasterBase::setCurrent(Constrain(current, -20000, 20000));
        }
    };
}    // namespace Udon