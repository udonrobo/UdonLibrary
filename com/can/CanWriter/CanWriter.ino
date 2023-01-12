#include "CanWriter.hpp"
#include <Message.hpp>

CanBus<CAN1>

CanWriter<Message::Motor> writer(bus, 0);

void setup() {
}

void loop() {
	writer.setMessage({ -124 }); /// データセット
	writer.update();        /// 配信
	delay(10);
}
