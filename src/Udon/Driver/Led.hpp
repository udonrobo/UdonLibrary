//
//    LED
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#ifdef ARDUINO

namespace Udon
{
    /// @brief LED制御クラス
    class Led
    {
        uint8_t pin;

    public:
        Led(uint8_t pin)
            : pin(pin)
        {
        }

        void begin() const
        {
            pinMode(pin, OUTPUT);
        }

        void high() const
        {
            digitalWrite(true);
        }

        void low() const
        {
            digitalWrite(false);
        }

        void digitalWrite(bool value) const
        {
            ::digitalWrite(pin, value ? HIGH : LOW);
        }

        void analogWrite(uint8_t value) const
        {
            ::analogWrite(pin, value);
        }

        /// @brief 時間を基に点滅
        /// @param intervalMs 周期
        /// @param onTimeMs 点灯時間
        void flush(uint32_t intervalMs = 500, uint32_t onTimeMs = 50) const
        {
            digitalWrite(millis() % intervalMs < onTimeMs);
        }

        /// @brief 時間を基にsin波のように点滅
        /// @param intervalMs 周期
        void wave(uint32_t intervalMs = 500) const
        {
            analogWrite(abs(map(millis() % intervalMs, 0, intervalMs, 0, 510) - 255));
        }
    };
}    // namespace Udon

#endif