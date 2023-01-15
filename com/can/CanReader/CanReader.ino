#include "CanCommon.hpp"

CanBusTeensy<CAN1> bus;
CanReader<Message::Motor> reader(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	if (reader)
	{
		Serial.println(reader.getMessage().power);
	}
	else
	{
		Serial.println("error");
	}
	bus.update();
	delay(10);
}
