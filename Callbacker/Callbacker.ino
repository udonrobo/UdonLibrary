#include "Callbacker.h"

class Sample : private Callbacker {
	void callback() override
	{
		Serial.println("call");
	}
};

void setup() {
	Serial.begin(115200);
	Sample sample[5] = {};
	Callbacker::bind();  /// 5回登録したので5回callと出力される
}

void loop() {
}
