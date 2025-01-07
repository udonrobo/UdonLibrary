//
//    モータードライバ制御クラス
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#ifdef ARDUINO

#    include <Arduino.h>
#    include <Udon/Algorithm/MovingAverage.hpp>
#    include <Udon/Types/Direction.hpp>

namespace Udon
{

    /// @brief スムーズにモーターを制御するクラス
    /// @tparam SmoothLevel 平滑化レベル (移動平均のサンプル数)
    template <size_t SmoothLevel>
    class SmoothlyMotor2
    {
        const uint8_t pinA;
        const uint8_t pinP;

        Udon::Direction direction;

        Udon::MovingAverage<SmoothLevel> ma;

    public:

        /// @brief コンストラクタ
        /// @param pinA 方向ピン
        /// @param pinP PWM ピン
        SmoothlyMotor2(const uint8_t pinA, const uint8_t pinP, Udon::Direction direction = Udon::Direction::Forward)
            : pinA(pinA)
            , pinP(pinP)
            , direction(direction)
            , ma{}
        {
        }

        /// @brief 出力開始
        void begin()
        {
            pinMode(pinA, OUTPUT);
        }

        /// @brief モーターを動かす
        /// @param power パワー (-250 ～ 250)
        void move(const int16_t power)
        {
            const int16_t p = ma(constrain(power, -250, 250)) * Udon::DirectionToSign(direction);
            digitalWrite(pinA, p >= 0 ? HIGH : LOW);
            analogWrite(pinP, abs(p));
        }

        /// @brief モーターを停止
        void stop()
        {
            move(0);
        }

        /// @brief パワーをシリアル出力
        void show() const
        {
            Serial.print(ma.getValue());
            Serial.print('\t');
        }
    };

    /// @brief スムーズにモーターを制御するクラス
    /// @tparam SmoothLevel 平滑化レベル (移動平均のサンプル数)
    template <size_t SmoothLevel>
    class SmoothlyMotor3
    {
        const uint8_t pinA;
        const uint8_t pinB;
        const uint8_t pinP;

        Udon::Direction direction;

        Udon::MovingAverage<SmoothLevel> ma;

    public:

        /// @brief コンストラクタ
        /// @param pinA 方向ピン
        /// @param pinB 方向ピン
        /// @param pinP PWM ピン
        /// @param direction 回転方向
        SmoothlyMotor3(const uint8_t pinA, const uint8_t pinB, const uint8_t pinP, Udon::Direction direction = Udon::Direction::Forward)
            : pinA(pinA)
            , pinB(pinB)
            , pinP(pinP)
            , direction(direction)
            , ma{}
        {
        }

        /// @brief 出力開始
        void begin()
        {
            pinMode(pinA, OUTPUT);
            pinMode(pinB, OUTPUT);
        }

        /// @brief モーターを動かす
        /// @param power パワー (-250 ～ 250)
        void move(const int16_t power)
        {
            const int16_t p = ma(constrain(power, -250, 250)) * Udon::DirectionToSign(direction);
            digitalWrite(pinA, p >= 0 ? HIGH : LOW);
            digitalWrite(pinB, p <= 0 ? HIGH : LOW);
            analogWrite(pinP, abs(p));
        }

        /// @brief モーターを停止
        void stop()
        {
            move(0);
        }

        /// @brief パワーをシリアル出力
        void show() const
        {
            Serial.print(ma.getValue());
            Serial.print('\t');
        }
    };

    using Motor2 = SmoothlyMotor2<50>;
    using Motor3 = SmoothlyMotor3<50>;

#ifdef __AVR_ATmega328P__

    /// @brief pwm周期変更 (atmega328p)
    inline void ChangePwmPeriod()
    {
        TCCR1B &= B11111000;
        TCCR1B |= B00000001;
        TCCR2B &= B11111000;
        TCCR2B |= B00000001;
    }

#endif

}    // namespace Udon

#endif