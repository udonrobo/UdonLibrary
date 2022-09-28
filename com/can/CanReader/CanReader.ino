#include "CanReader.h"

CanReader<10> reader1(1);
CanReader<10> reader1(2);
CanReader<10> reader1(3);

void setup() {
	Serial.begin(115200);
}

void loop() {
	reader1.show('\n');
	delay(10);
}
