#include "CanBus.hpp"
#include "CanWriter.hpp"
#include <Message.hpp>

struct Hoge {
	uint8_t data[20];
};

CanBus<CAN1> bus;
CanWriter<Hoge> writer(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	Hoge hoge;
	for (auto && it : hoge.data) {
		it = millis();
	}
	writer.setMessage(hoge);

	bus.update();
	delay(10);
}
