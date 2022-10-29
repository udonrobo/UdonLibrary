#include "CanWriter.h"
#include "CanReader.h"

CanWriter<10> writer(1);
CanReader< 5> reader(2);

void setup() {
}

void loop() {
	reader.show('\n');
	
	writer = millis();
	writer.update();
	
	delay(1);
}
