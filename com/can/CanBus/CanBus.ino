#include "CANCommon.hpp"

#include <vector>


CANBusSPI<0, 0> bus(SPI);

CANBusTeensy <CAN1> bubu;

CANWriter<Message::Motor> writers[] {
	{ bus, 0 },
	{ bus, 1 },
	{ bus, 2 },
	{ bus, 3 },
	{ bus, 4 },
	{ bus, 5 },
	{ bus, 6 }
};

CANReader<Message::RGB> rgb(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	rgb.getMessage().show();
	for (auto && writer : writers) {
		writer.setMessage( { (int16_t)millis() } );
		if (writer)
		{
			writer.show();
		}
		else
		{
			Serial.println("error");
		}
	}
	Serial.println(bus.getErrorInfo());

	bus.update();
	delay(1);
}
