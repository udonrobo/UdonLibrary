#include "CanCommon.hpp"

CanBusTeensy<CAN1> bus;

CanWriter<int> writer(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	bus.update();
}
