#include "CanCommon.hpp"

#include <vector>

CanBusTeensy<CAN1> bus;

CanWriter<Message::Motor> writers[] {
	{ bus, 0 },
	{ bus, 1 },
	{ bus, 2 },
	{ bus, 3 },
	{ bus, 4 },
	{ bus, 5 },
	{ bus, 6 }
};

CanReader<Message::RGB> rgb(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	rgb.getMessage().show();
	for (auto && writer : writers) {
		writer.setMessage( { millis() } );
		if (writer)
		{
			writer.show();
		}
		else
		{
			Serial.println("error");
		}
	}
	Serial.println(bus.getErrorHandle());

	bus.update();
	delay(1);
}
