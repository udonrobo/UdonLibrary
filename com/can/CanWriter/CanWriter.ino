#include "CanWriter.h"

CanWriter<255> writer1(1);
CanWriter<255> writer2(2);
CanWriter<255> writer3(3);

void setup() {
}

void loop() {
	auto t = micros();
	writer1 = millis() / 10; /// データセット
	writer1.update();        /// 配信
	writer2 = millis() / 100; /// データセット
	writer2.update();        /// 配信
	writer3 = millis() / 1000; /// データセット
	writer3.update();        /// 配信
	Serial.println(micros() - t);
	delay(10);
}
