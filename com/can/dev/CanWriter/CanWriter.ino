#include "CanWriter.hpp"

CanBus can;
CanWriter<10> writer(can, 10);

struct ToPc {
	uint8_t data[10];
};

void setup() {
	ToPc to;
	writer.setDataFrame(to);
}

void loop() {
	// put your main code here, to run repeatedly:

}
