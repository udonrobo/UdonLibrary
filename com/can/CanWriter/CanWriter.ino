#include "CanWriter.hpp"

CanWriter<USE_MOTOR(226)> writer1(1);

void setup() {
}

void loop() {
	auto t = micros();

	for (int i = 0; i < 226; i++)
		writer1.setMotorData(i, -100); /// データセット
	writer1.update();        /// 配信

	Serial.println(writer1.motorSize());

	delay(10);
}
