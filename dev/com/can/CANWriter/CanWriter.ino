#include <CanCommon.hpp>

#include <vector>

CanBusTeensy<CAN1> bus;
std::vector<CanWriter<Message::Motor>> writers {
	{ bus, 0 },
	{ bus, 1 },
	{ bus, 2 },
	{ bus, 3 },
	{ bus, 4 },
};

void setup() {
	for(auto&& it : writers) {
		bus.join(it);
	}
	bus.begin();
}

void loop() {
	for(auto&& it : writers) {
		it.setMessage({ millis() });
	}
	bus.update();
	delay(10);
}
