#pragma once

#ifdef ARDUINO

namespace Udon
{
    class LedDriver
    {
        uint8_t pin;

    public:
        LedDriver(uint8_t pin)
            : pin(pin)
        {
        }

        void begin() const
        {
            pinMode(pin, OUTPUT);
        }

        void on() const
        {
            digitalWrite(true);
        }

        void off() const
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
        void flush(uint32_t intervalMs = 500, uint32_t onTimeMs = 50) const
        {
            digitalWrite(millis() % intervalMs < onTimeMs);
        }

        /// @brief 時間を基にsin波のように点滅
        /// @param intervalMs 
        void wave(uint32_t intervalMs = 500) const
        {
            analogWrite(abs(map(millis() % intervalMs, 0, intervalMs, 0, 510) - 255));
        }
    };
}    // namespace Udon

#endif