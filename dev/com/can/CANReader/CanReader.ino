#include <CanCommon.hpp>

#include <vector>

CanBusTeensy<CAN1> bus;
std::vector<CanReader<Message::Motor>> readers {
	{ bus, 0 },
	{ bus, 1 },
	{ bus, 2 },
	{ bus, 3 },
	{ bus, 4 },
};

void setup() {
	for(auto&& it : readers) {
		bus.join(it);
	}
	bus.begin();
}

void loop() {
	for(auto&& it : readers) {
		it.show();
	}
	Serial.println();
	bus.update();
	delay(10);
}
