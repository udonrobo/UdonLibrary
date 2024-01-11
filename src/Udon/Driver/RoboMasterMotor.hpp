#pragma once

#include <Udon/Com/Can.hpp>

namespace Udon
{

    class RoboMasterMotor
    {
        uint8_t       txBuffer[8]{};
        Udon::CanNode robomaster;

        uint8_t       rxBuffer[8]{};
        Udon::CanNode motorstatus;

        uint8_t motorId;

        int32_t offset;
        int32_t prevAngle;

    public:
        RoboMasterMotor(Udon::ICanBus& bus, uint8_t motorId)
            : robomaster{
                .id         = 0x200,
                .data       = txBuffer,
                .length     = sizeof txBuffer,
                .transmitMs = 0
            }
            , motorstatus{
                .id         = 0x200 + constrain(motorId, 1, 8),
                .data       = rxBuffer,
                .length     = sizeof rxBuffer,
                .transmitMs = 0,
            }
            , motorId(motorId)
        {
            bus.joinRx(
                motorstatus, [](void* p)
                {
            auto self = static_cast<RoboMasterMotor*>(p);
            const auto currentAngle = self->motorstatus.data[0] << 8 | self->motorstatus.data[1];

            // 変化角
            const auto dTheta = self->prevAngle - currentAngle;
            self->prevAngle = currentAngle;

            // 変化量がいきなり半周を超えた -> 計算値が-π~π間を通過 -> 一周分オフセットを加減算
            const auto ppr = 8191;
            if (dTheta > ppr / 2) {
            self->offset += ppr;
            } else if (dTheta < -ppr / 2) {
            self->offset -= ppr;
            } },
                this);
            bus.joinTx(robomaster);
        }

        void setCurrent(int16_t current)
        {
            const auto motorIndex    = (motorId - 1) * 2;
            txBuffer[motorIndex]     = highByte(current);
            txBuffer[motorIndex + 1] = lowByte(current);
        }

        int32_t getCount() const
        {
            return prevAngle + offset;
        }

        uint16_t getSpeed() const
        {
            return motorstatus.data[2] << 8 | motorstatus.data[3];
        }

        uint16_t getCurrent() const
        {
            return motorstatus.data[4] << 8 | motorstatus.data[5];
        }

        uint8_t getTemperature() const
        {
            return motorstatus.data[6];
        }
    };

}    // namespace Udon