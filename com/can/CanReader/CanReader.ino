#include "CanReader.hpp"
#include <Message.hpp>

CanReader<Message::Motor> reader(0);

void setup() {
	Serial.begin(115200);
}

void loop() {
	if (reader.isOpen()) {

		const auto data = reader.getMessage();  /// 受信データ表示

		Serial.println(data.power);

	}
	delay(10);
}
