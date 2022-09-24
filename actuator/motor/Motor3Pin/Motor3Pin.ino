#include "Motor3.h"
Motor motor[] = {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 9}
};

void setup() {
	setPwmTimer();
}

void loop() {
	for (const auto& motor : motor) {
		motor.move(50);
	}
}
