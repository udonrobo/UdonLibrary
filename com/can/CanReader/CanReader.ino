#include "CanReader.hpp"
#include "CanBus.hpp"
#include "Message.hpp"

#include <array>

CanBusTeensy<CAN1> bus;

std::array<CanReader<Message::Motor>, 6> reader {
	CanReader<Message::Motor>{ bus, 0 },
	CanReader<Message::Motor>{ bus, 1 },
	CanReader<Message::Motor>{ bus, 2 },
	CanReader<Message::Motor>{ bus, 3 },
	CanReader<Message::Motor>{ bus, 4 },
	CanReader<Message::Motor>{ bus, 5 }
};

void setup() {
	bus.begin();
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	for (auto && it : reader) {
		it.show();
	}
	Serial.println();
	bus.update();
	delay(10);

	digitalWrite(LED_BUILTIN, millis() % 500 < 100);
}
