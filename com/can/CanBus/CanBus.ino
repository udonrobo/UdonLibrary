#include "CanCommon.hpp"

#include <vector>

CanBusTeensy<CAN1> bus;

CanWriter<Message::Motor> writer{ bus, 0 };

void setup() {
	bus.begin();
}

void loop() {
	writer.setMessage( { millis() } );
	if (writer)
	{
		writer.show('\n');
	}
	else
	{
		Serial.println("error");
	}
	bus.update();
	delay(10);
}
