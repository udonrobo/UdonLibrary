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

        void begin()
        {
            pinMode(pin, OUTPUT);
        }

        void on()
        {
            write(true);
        }

        void off()
        {
            write(false);
        }

        void write(bool value)
        {
            digitalWrite(pin, value ? HIGH : LOW);
        }
    };
}    // namespace Udon

#endif