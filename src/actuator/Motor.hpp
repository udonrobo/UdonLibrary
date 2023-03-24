/// @file   Motor.hpp
/// @date   2022/12/11
/// @brief  モタド制御クラス
/// @author 大河 祐介

#pragma once

#include <algorithm\MovingAverage.hpp>

class Motor2 : private MovingAverage {
		const uint8_t pinA;
		const uint8_t pinP;
	public:
		Motor2(const uint8_t pinA, const uint8_t pinP)
			: MovingAverage(20)
			, pinA(pinA)
			, pinP(pinP)
		{
			pinMode(pinA, OUTPUT);
		}

		void move(const int16_t power) {
			MovingAverage::Update(constrain(power, -250, 250));
			const int16_t p = MovingAverage::GetValue();
			digitalWrite(pinA, p >= 0);
			analogWrite (pinP, abs(p));
		}
};

class Motor3 : private MovingAverage {
		const uint8_t pinA;
		const uint8_t pinB;
		const uint8_t pinP;
	public:
		Motor3(const uint8_t pinA, const uint8_t pinB, const uint8_t pinP)
			: MovingAverage(20)
			, pinA(pinA)
			, pinB(pinB)
			, pinP(pinP)
		{
			pinMode(pinA, OUTPUT);
			pinMode(pinB, OUTPUT);
		}

		void move(const int16_t power) {
			MovingAverage::Update(constrain(power, -250, 250));
			const int16_t p = MovingAverage::GetValue();
			digitalWrite(pinA, p >= 0);
			digitalWrite(pinB, p <= 0);
			analogWrite (pinP, abs(p));
		}
};

// pwm周期変更 (arduino nano用)
// TCCR1B &= B11111000;
// TCCR1B |= B00000001;
// TCCR2B &= B11111000;
// TCCR2B |= B00000001;
