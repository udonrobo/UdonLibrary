#include "CanReader.h"

CanReader<255> reader1(1);

void setup() {
	Serial.begin(115200);
}

void loop() {
	reader1.show('\n');  /// 受信

	delay(100);
}
