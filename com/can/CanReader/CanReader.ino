#include "CanReader.h"

CanReader<10> reader1(1);

void setup() {
	Serial.begin(115200);
}

void loop() {
	reader1.show('\n');
	delay(10);
}
