#define ENABLE_STL

#include "CanWriter.hpp"
#include <Message.hpp>

#include <sstream>

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

void show() {
	std::stringstream ss;
	ss << writer;
	Serial.println(ss.str().c_str());
}
