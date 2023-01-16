#include "CanReader.hpp"
#include "CanBus.hpp"
#include "Message.hpp"

#include <vector>

CanBusTeensy<CAN1> bus;
CanReader<Message::Motor> reader[]  {
	{ bus, 0 },
	{ bus, 1 },
	{ bus, 2 },
	{ bus, 3 },
	{ bus, 4 },
	{ bus, 5 }
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
