#include "CanWriter.hpp"
#include <Message.hpp>

CanWriter<sizeof(Message::Motor)> writer(0);

void setup() {
}

void loop() {
	writer.setMessage(Message::Motor{ -124 }); /// データセット
	writer.update();        /// 配信
	delay(10);
}
