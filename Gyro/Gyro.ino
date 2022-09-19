#include "Gyro.h"

Gyro turn;
Gyro positive;

void setup() {
	Serial.begin(115200);
	Gyro::begin();
}

void loop() {
	Gyro::update();

	static uint32_t hold;
	if (millis() - hold > 1000)
	{
		hold = millis();
		turn.clear();
	}

	Serial.print(turn.yaw());     Serial.print('\t');
	Serial.print(positive.yaw()); Serial.print('\n');

	delay(10);
}
