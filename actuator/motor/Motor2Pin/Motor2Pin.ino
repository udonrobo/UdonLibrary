#include "Motor2.h"
Motor motor[] = {
	{1, 2},
	{4, 5},
	{7, 8}
};

void setup() {
}

void loop() {
	for (const auto& motor : motor) {
		motor.move(50);
	}
}
