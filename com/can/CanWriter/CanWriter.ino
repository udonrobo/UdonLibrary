#include "CanWriter_t4x.h"

CanWriter<4> writer(1);  /// 送信先のIDを登録

void setup() {
	Serial.begin(115200);
	CanBase::begin(0);   /// 自分のIDを登録
}

void loop() {
	writer.update();
	writer[0] = millis() / 1;
	writer[1] = millis() / 10;
	writer[2] = millis() / 100;
	writer[3] = millis() / 1000;
	delay(10);
}
