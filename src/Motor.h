/// @file   Motor.h
/// @date   2022/12/05
/// @brief  モタド制御クラス
/// @author 大河 祐介

#pragma once
#include "MovingAverage.h"

struct DriverType_Black_Tag;
struct DriverType_Tanikawa_Tag;

template<class> class Motor;

/// @brief 黒モタド (2ピン)
template<>
class Motor<DriverType_Black_Tag> : private MovingAverage {
		const uint8_t pinA;
		const uint8_t pinP;
	public:
		Motor(const uint8_t pinA, const uint8_t pinP)
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


/// @brief 谷川さんモタド (3ピン)
template<>
class Motor<DriverType_Tanikawa_Tag> : private MovingAverage {
		const uint8_t pinA;
		const uint8_t pinB;
		const uint8_t pinP;
	public:
		Motor(const uint8_t pinA, const uint8_t pinB, const uint8_t pinP)
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

using Motor2 = Motor<DriverType_Black_Tag   >;
using Motor3 = Motor<DriverType_Tanikawa_Tag>;

// pwm周期変更 (arduino nano)
// TCCR1B &= B11111000;
// TCCR1B |= B00000001;
// TCCR2B &= B11111000;
// TCCR2B |= B00000001;
