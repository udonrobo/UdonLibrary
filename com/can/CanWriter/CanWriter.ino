#include "CanWriter_t4x.h"

CanWriter<7> writer(12);

void setup() {
	Serial.begin(115200);
}

void loop() {
	writer.update();
	writer[0] = millis();
	delay(10);
}
