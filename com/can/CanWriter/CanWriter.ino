#include "CanWriter.h"

CanWriter<10> writer1(1);  /// 送信先のIDを登録
CanWriter<10> writer2(2);  /// 送信先のIDを登録
CanWriter<10> writer3(3);  /// 送信先のIDを登録

void setup() {
	Serial.begin(115200);
}

void loop() {
	const auto n = micros();
	writer1[0] = millis() / 10;
	writer1.update();
	writer2[0] = millis() / 100;
	writer2.update();
	writer3[0] = millis() / 1000;
	writer3.update();
	Serial.println(micros() - n);
	delay(10);
}
