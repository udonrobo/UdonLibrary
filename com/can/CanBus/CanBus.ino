#include "CanCommon.hpp"

CanBusSpi<10, 2> bus(SPI, 8000000);
CanReader<Message::Motor> reader{ bus, 0 };

void setup() {
	delay(100);
	Serial.begin(115200);
	bus.begin();
}

void loop() {
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
