#include "CanReader.hpp"

CanBus<CAN1> can;
CanReader<10> reader(can, 10);

void setup() {
	can.begin();
}

void loop() {
}
