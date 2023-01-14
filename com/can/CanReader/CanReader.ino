#include "CanReader.hpp"
#include "CanBus.hpp"
#include <Message.hpp>

struct Hoge {
	uint8_t data[20];
};

CanBus<CAN1> bus;
CanReader<Hoge> reader0(bus, 0);


void setup() {
	bus.begin();
}

void loop() {
	for (auto && it : reader0.getMessage().data) {
		Serial.print(it);  /// 受信データ表示
		Serial.print('\t');
	}
	Serial.println();
	delay(10);
}
