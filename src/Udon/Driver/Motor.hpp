//
//    モータードライバ制御クラス
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#ifdef ARDUINO

#    include <Arduino.h>
#    include <Udon/Algorithm/MovingAverage.hpp>

namespace Udon
{

    template <size_t SmoothLevel>
    class SmoothlyMotor2
    {
        const uint8_t pinA;
        const uint8_t pinP;

        Udon::MovingAverage<SmoothLevel> movingAverage;

    public:
        SmoothlyMotor2(const uint8_t pinA, const uint8_t pinP)
            : pinA(pinA)
            , pinP(pinP)
            , movingAverage{}
        {
        }

        void begin()
        {
            pinMode(pinA, OUTPUT);
        }

        void move(const int16_t power)
        {
            const int16_t p = movingAverage(constrain(power, -250, 250));
            digitalWrite(pinA, p >= 0 ? HIGH : LOW);
            analogWrite(pinP, abs(p));
        }

        void stop()
        {
            move(0);
        }

        void show() const
        {
            Serial.print(movingAverage.getValue());
            Serial.print('\t');
        }
    };

    template <size_t SmoothLevel>
    class SmoothlyMotor3
    {
        const uint8_t pinA;
        const uint8_t pinB;
        const uint8_t pinP;

        Udon::MovingAverage<SmoothLevel> movingAverage;

    public:
        SmoothlyMotor3(const uint8_t pinA, const uint8_t pinB, const uint8_t pinP)
            : pinA(pinA)
            , pinB(pinB)
            , pinP(pinP)
            , movingAverage{}
        {
        }

        void begin()
        {
            pinMode(pinA, OUTPUT);
            pinMode(pinB, OUTPUT);
        }

        void move(const int16_t power)
        {
            movingAverage.update(constrain(power, -250, 250));
            const int16_t p = movingAverage.getValue();
            digitalWrite(pinA, p >= 0 ? HIGH : LOW);
            digitalWrite(pinB, p <= 0 ? HIGH : LOW);
            analogWrite(pinP, abs(p));
        }

        void stop()
        {
            move(0);
        }

        void show() const
        {
            Serial.print(movingAverage.getValue());
            Serial.print('\t');
        }
    };

    using Motor2 = SmoothlyMotor2<50>;
    using Motor3 = SmoothlyMotor3<50>;

#ifdef __AVR_ATmega328P__

    /// @brief pwm周期変更 (atmega328p)
    inline void ArduinoNanoPwmPeriodChange()
    {
        TCCR1B &= B11111000;
        TCCR1B |= B00000001;
        TCCR2B &= B11111000;
        TCCR2B |= B00000001;
    }

#endif

}    // namespace Udon

#endif