#pragma once

#include <Udon/Com/Can.hpp>
#include <Udon/Algorithm/Math.hpp>

namespace Udon
{

    class RoboMasterC620
    {
    public:

        /// @brief コンストラクタ
        /// @param bus CAN通信バス
        /// @param motorId モーターID (1~8)
        RoboMasterC620(Udon::ICanBus& bus, uint8_t motorId, bool direction = true)
            : bus{ bus }
            , motorId{ motorId }
            , direction{ direction }
            , rx{ static_cast<uint32_t>(0x200 + Constrain(motorId, 1, 8)), rxBuffer, sizeof rxBuffer, 0 }
        {
            if (bus.findTx(shared.node.id, shared.node.length) == nullptr)
            {
                // 送信バッファが見つからない場合は新規作成
                bus.joinTx(shared.node);
                useSharedBuffer = true;
            }

            bus.joinRx(rx, onReceive, this);
        }

        RoboMasterC620(const RoboMasterC620& other)
            : bus{ other.bus }
            , motorId{ other.motorId }
            , direction{ other.direction }
            , rx{ static_cast<uint32_t>(0x200 + motorId), rxBuffer, sizeof rxBuffer, 0 }
        {
            if (bus.findTx(shared.node.id, shared.node.length) == nullptr)
            {
                // 送信バッファが見つからない場合は新規作成
                bus.joinTx(shared.node);
                useSharedBuffer = true;
            }
            bus.joinRx(rx, onReceive, this);
        }

        ~RoboMasterC620()
        {
            if (useSharedBuffer)
            {
                bus.leaveTx(shared.node);   // ※1 ここで参照が切れる
            }
            bus.leaveRx(rx);
        }

        explicit operator bool() const
        {
            return Millis() - rx.transmitMs < 100;
        }

        /// @brief モーターの電流を設定
        /// @param current 電流値 [-20000, 20000] (単位: mA)
        void setCurrent(int16_t current)
        {
            current = Constrain(current, -20000, 20000) * directionSign();

            const auto transmitCurrent = static_cast<int16_t>(current * 16384 / 20000);    // 16bit符号なし整数に変換
            const auto motorIndex      = (motorId - 1) * 2;
            if (useSharedBuffer)
            {
                shared.data[motorIndex + 0] = transmitCurrent >> 8 & 0xff;    // high byte
                shared.data[motorIndex + 1] = transmitCurrent >> 0 & 0xff;    // low byte
            }
            else
            {
                auto reference                  = bus.findTx(shared.node.id, shared.node.length);    // コピーによって参照が切れるのを防ぐため、毎回検索を行う (※1)
                reference->data[motorIndex + 0] = transmitCurrent >> 8 & 0xff;                       // high byte
                reference->data[motorIndex + 1] = transmitCurrent >> 0 & 0xff;                       // low byte
            }
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
        uint16_t getVelocity() const
        {
            return (rxBuffer[2] << 8 | rxBuffer[3]) * directionSign();
        }


        /// @brief モーターのトルク電流を取得
        /// @return トルク電流 [mA]
        int16_t getTorqueCurrent() const
        {
            const int current = rxBuffer[4] << 8 | rxBuffer[5];
            return current * directionSign();
        }


        /// @brief モーターの温度を取得
        /// @return 温度 [℃]
        uint8_t getTemperature() const
        {
            return rxBuffer[6];
        }

    private:
        /// @brief 通信バス
        Udon::ICanBus& bus;


        /// @brief モーターID
        uint8_t motorId;


        /// @brief 回転方向
        bool direction;


        /// @brief 送信バッファ
        struct
        {
            uint8_t       data[8];
            Udon::CanNode node{ 0x200, data, sizeof data, 0 };
        } shared;    // 共有先のバッファ (送信バッファが共有されている場合のみ使用)
        bool useSharedBuffer = false;


        /// @brief 受信バッファ
        uint8_t       rxBuffer[8]{};
        Udon::CanNode rx;


        /// @brief オフセット角度 (生の角度は無限の角度を扱えないので、無限の角度を扱えるようにオフセットを加減算する)
        int64_t offsetAngle = 0;
        int64_t angle       = 0;


        /// @brief エンコーダー分解能
        static constexpr int32_t ppr = 8192;

        static void onReceive(void* p)
        {
            auto self = static_cast<RoboMasterC620*>(p);

            // 変化角を計算
            const auto currentAngle = self->rxBuffer[0] << 8 | self->rxBuffer[1];
            const auto dTheta       = self->angle - currentAngle;
            self->angle             = currentAngle;

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

        int directionSign() const
        {
            return direction ? 1 : -1;
        }
    };
}    // namespace Udon