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

    class Motor2Driver
        : MovingAverage
    {
        const uint8_t pinA;
        const uint8_t pinP;

    public:
        Motor2Driver(const uint8_t pinA, const uint8_t pinP)
            : MovingAverage(20)
            , pinA(pinA)
            , pinP(pinP)
        {
            pinMode(pinA, OUTPUT);
        }

        void move(const int16_t power)
        {
            MovingAverage::update(constrain(power, -250, 250));
            const int16_t p = MovingAverage::getValue();
            digitalWrite(pinA, p >= 0);
            analogWrite(pinP, abs(p));
        }

        void stop()
        {
            move(0);
        }

        void show() const
        {
            Serial.print(MovingAverage::getValue());
            Serial.print('\t');
        }
    };

    class Motor3Driver
        : MovingAverage
    {
        const uint8_t pinA;
        const uint8_t pinB;
        const uint8_t pinP;

    public:
        Motor3Driver(const uint8_t pinA, const uint8_t pinB, const uint8_t pinP)
            : MovingAverage(20)
            , pinA(pinA)
            , pinB(pinB)
            , pinP(pinP)
        {
            pinMode(pinA, OUTPUT);
            pinMode(pinB, OUTPUT);
        }

        void move(const int16_t power)
        {
            MovingAverage::update(constrain(power, -250, 250));
            const int16_t p = MovingAverage::getValue();
            digitalWrite(pinA, p >= 0);
            digitalWrite(pinB, p <= 0);
            analogWrite(pinP, abs(p));
        }

        void stop()
        {
            move(0);
        }

        void show() const
        {
            Serial.print(MovingAverage::getValue());
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