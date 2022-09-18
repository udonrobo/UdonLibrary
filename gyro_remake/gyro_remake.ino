#include "Gyro.h"

Gyro gyro;
Gyro a;

void setup() {
	Serial1.begin(115200);
	Serial.begin(115200);
	Gyro::update();
	a.clear();
}

void loop() {
	Gyro::update();
	
	Serial.print(a.yaw()),    Serial.print('\t');
	Serial.print(gyro.yaw()), Serial.print('\n');
	
	delay(10);
}
