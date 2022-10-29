#include "CanWriter.h"

CanWriter<10> writer1(1);

void setup() {
}

void loop() {
	writer1 = millis() / 10; /// データセット
	writer1.update();        /// 配信
	delay(10);
}
