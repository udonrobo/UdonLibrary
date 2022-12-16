#include "CanReader.hpp"

CanReader<10> reader(0);

void setup() {
	Serial.begin(115200);
}

void loop() {
	reader.showMotor('\n');  /// 受信データ表示
	delay(10);
}
