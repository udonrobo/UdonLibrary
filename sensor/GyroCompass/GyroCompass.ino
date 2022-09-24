#include "QMC5883LCompass.h"

QMC5883LCompass compass;

void setup() {
	Serial.begin(115200);
	compass.setMode(0x01, 0x0C, 0x10, 0x00);
	compass.init();
	compass.setSmoothing(100, true);
	compass.setCalibration(-1252, 1070, -1407, 1062, -1727, 0);
}

void loop() {
	compass.read();
	Serial.print('\t'); Serial.print(0);
	Serial.print('\t'); Serial.print(360);
	Serial.print('\t'); Serial.println(compass.getAzimuth());

	delay(1);
}
