#include <Wire.h>
#include "FunctionBinder.h"

class Reader : FunctionBinder<void(int)>
{
	public:
		Reader() {
			Wire.onReceive(FunctionBinder<void(int)>::bind);
		}
		void callback(int) override {
			Serial.println("received");
		}
};

void setup() {
	Serial.begin(115200);
	Wire.begin(0);
}
void loop() {
	/*
		受信割り込みのためloop内は空
		received
		received
		received
		received
	*/
}
