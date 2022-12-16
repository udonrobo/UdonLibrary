#define ENABLE_STL

#include "CanWriter.hpp"
#include <Message.hpp>

CanWriter<sizeof(Message::Motor)> writer(1);

void setup() {
}

void loop() {
	Message::Motor motor;
	motor.power = 100;

	writer.setMessage(motor); /// データセット

	writer.update();        /// 配信

	delay(10);
}
