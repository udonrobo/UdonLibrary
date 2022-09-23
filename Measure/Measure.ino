#include "Measure.h"

EncoderBoardTeensy enc(2);
Measure measure(enc, 1, 2, { false, true });

void setup() {
	Wire.begin();
	Wire.setClock(400000UL);
	Serial.begin(115200);
	Gyro::begin(115200);
	enc.init();
}

void loop() {
	Gyro::update();
	enc.update();
	measure.update();

	//	Serial.print(measure.x()); Serial.print('\t');
	//	Serial.print(measure.y()); Serial.print('\n');
	measure.show('\n');
	delay(10);
}
