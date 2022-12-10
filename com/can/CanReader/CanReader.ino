#include "CanReader.hpp"

CanReader<10> reader1(1);
CanReader<10> reader2(1);

void setup() {
	Serial.begin(115200);
}

void loop() {
	reader1.show('\n');  /// 受信データ表示
	reader2.show('\n');  /// 受信データ表示
	delay(10);
}
