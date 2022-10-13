#include "FunctionBinder.h"

struct sample : private FunctionBinder<void()> {
	void callback() override {
		Serial.print("call");
	}
};

void setup() {
	Serial.begin(115200);
}

sample s[9] = {};

void loop() {
	//	reader1.show('\n');

	FunctionBinder<void()>::bind();
	//	Serial.println();
	delay(10);
}
