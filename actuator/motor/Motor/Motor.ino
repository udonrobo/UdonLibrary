#include "Motor.h"

Motor3 motor[] {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 9}
};

void setup() {
}

void loop() {
	for (const auto& m : motor) {
		m.move(50);
	}
}
