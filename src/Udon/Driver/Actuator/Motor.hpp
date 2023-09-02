//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    モータードライバ制御クラス
//
//-------------------------------------------------------------------

#pragma once

#ifdef ARDUINO

#    include <Arduino.h>
#    include <Udon/Algorithm/MovingAverage.hpp>

namespace Udon
{

    class Motor2
    {
        const uint8_t pinA;
        const uint8_t pinP;

        Udon::MovingAverage<30> movingAverage;

    public:
        Motor2(const uint8_t pinA, const uint8_t pinP)
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
            movingAverage.update(constrain(power, -250, 250));
            const int16_t p = movingAverage.getValue();
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

    class Motor3
    {
        const uint8_t pinA;
        const uint8_t pinB;
        const uint8_t pinP;

        Udon::MovingAverage<30> movingAverage;

    public:
        Motor3(const uint8_t pinA, const uint8_t pinB, const uint8_t pinP)
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

}    // namespace Udon

// pwm周期変更 (arduino nano用)
// TCCR1B &= B11111000;
// TCCR1B |= B00000001;
// TCCR2B &= B11111000;
// TCCR2B |= B00000001;

#endif