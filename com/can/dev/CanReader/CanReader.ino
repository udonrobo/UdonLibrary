#include "CanBus.hpp"

CanBus<CAN1> can;
//CanReader<10, CAN1> reader(can, 10);

void setup() {
	can.begin();
}

void loop() {
	f(100);
	f(100.0);
}
