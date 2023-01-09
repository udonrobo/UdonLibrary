#include "CanWriter.hpp"
#include <Message.hpp>

CanWriter<Message::Motor> writer(0);

void setup() {
}

void loop() {
	writer.setMessage({ -124 }); /// データセット
	writer.update();        /// 配信
	delay(10);
}
