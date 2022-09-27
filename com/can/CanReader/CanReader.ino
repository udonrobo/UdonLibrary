#include "CanReader.h"

CanReader<6> reader1(1);
CanReader<6> reader2(2);
CanReader<6> reader3(3);

void setup() {
	Serial.begin(115200);
}

void loop() {
	const auto n = micros();
	reader1.show();
	reader2.show();
	reader3.show();
	Serial.println(micros() - n);
	delay(10);
}
