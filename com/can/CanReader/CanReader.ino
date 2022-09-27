#include "CanReader_t4x.h"

CanReader<4> reader(0);

void setup() {
	Serial.begin(115200);
}

void loop() {
	reader.show('\n');
	delay(10);
}
