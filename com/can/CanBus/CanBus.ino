#include "CanCommon.hpp"

CanBusSpi<1, 2> bus(SPI);

CanWriter<Message::Motor> writer(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	bus.update();
}
