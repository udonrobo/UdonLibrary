#include "CanWriter.h"

CanWriter<5> writer1(1);  /// インスタンスID

void setup() {
}

void loop() {
	writer1 = millis() / 10;
	writer1.update();
	delay(10);
}
