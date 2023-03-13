#include "Gyro.hpp"

Gyro positive;

void setup() {
	Serial.begin(115200);
	Gyro::begin(115200);
}

void loop() {
	Gyro::update();

	Serial.print(positive.yaw()); Serial.print('\n');

	delay(10);
}
