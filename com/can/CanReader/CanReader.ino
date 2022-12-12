#include "CanReader.hpp"

CanReader<USE_MOTOR(200)> reader1(1);

void setup() {
	Serial.begin(115200);
}

void loop() {
	reader1.showMotor('\n');  /// 受信データ表示
	delay(10);
}
