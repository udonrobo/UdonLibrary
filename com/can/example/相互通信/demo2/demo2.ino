#include "CanWriter.h"
#include "CanReader.h"

CanWriter< 5> writer(2);
CanReader<10> reader(1);

void setup() {
}

void loop() {
	reader.show('\n');
	
	writer = millis() / 100;
	writer.update();
	
	delay(1);
}
