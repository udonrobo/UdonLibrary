#include "CanReader.hpp"
#include <Message.hpp>

PACK(struct Smaple {
	PACK(struct {
		bool click : 1;
	}) sw;
	uint32_t value;
});

CanReader<sizeof(Message::Motor)> reader(0);

void setup() {
	Serial.begin(115200);
}

void loop() {
	const auto data = reader.getMessage<Message::Motor>();  /// 受信データ表示
	Serial.println(data.power);
	delay(10);
}
