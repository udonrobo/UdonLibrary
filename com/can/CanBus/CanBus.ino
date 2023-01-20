#include "CanCommon.hpp"

#include <vector>

CanBusTeensy<CAN1> bus;

CanWriter<Message::Motor> reader{ bus, 0 };

void setup() {
	bus.begin();
}

void loop() {
	reader.setMessage( { millis() } );
	if (reader)
	{
		reader.show('\n');
	}
	else
	{
		Serial.println("error");
	}
	bus.update();
	delay(10);
}
