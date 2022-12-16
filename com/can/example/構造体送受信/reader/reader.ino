#incldue <CanReader.hpp>
#include <Message.hpp>

CanReader<sizeof(Message::Motor)> reader(0);

void setup() {
	Serial.begin(115200);
}

void loop() {
	const auto data = reader.getMessage<Message::Motor>();
	Serial.println(data.power);
}
