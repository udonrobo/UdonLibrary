#include "Measure.h"

EncoderBoardTeensy enc(0);
Measure measure(enc, 1, 2);

void setup() {
	Serial.begin(115200);
	Gyro::begin();
	enc.init();
}

void loop() {
	Gyro::update();
	measure.update();

	Serial.print(measure.x()); Serial.print('\t');
	Serial.print(measure.y()); Serial.print('\n');

	delay(10);
}
