#include "I2cSlaveWriter.h"

I2cSlaveWriter<4> sender(5);

void setup() {
}

void loop() {
	sender.setByteData(0, 123);
	delay(10);
}
