#include "CanCommon.hpp"

CanBusTeensy<CAN1> bus;
CanReader<Message::Motor> reader0(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	if (reader0)
	{
		Serial.println(reader0.getMessage().power);
	}
	else
	{
		Serial.println("error");
	}
	bus.update();
	delay(10);
}
