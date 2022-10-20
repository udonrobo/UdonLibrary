/// @file   Motor2.h
/// @date   2022/09/22
/// @brief  モタド制御クラス
/// @author 大河 祐介

#pragma once
#include "MovingAverage.h"

class Motor : private MovingAverage {
		const uint8_t pinA;
		const uint8_t pinP;
	public:
		Motor(const uint8_t pinA, const uint8_t pinP)
			: MovingAverage(20)
			, pinA(pinA)
			, pinP(pinP)
		{}

		void move(const int16_t power) {
			MovingAverage::Update(constrain(power, -250, 250));
			const int16_t movePower = MovingAverage::GetValue();
			digitalWrite(pinA, movePower >= 0);
			analogWrite (pinP, abs(movePower));
		}
};

// arduino nanoの場合以下をsetup内に記述(pwm周期変更)
// TCCR1B &= B11111000;
// TCCR1B |= B00000001;
// TCCR2B &= B11111000;
// TCCR2B |= B00000001;
