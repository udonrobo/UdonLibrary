#include "CanReader.h"

void setup() {
	Serial.begin(115200);
}

void loop() {
	//	reader1.show('\n');

	FunctionBinder<void()>::bind();
	//	Serial.println();
	delay(10);
}
