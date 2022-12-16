#include <ostream>
#include <sstream>

void setup() {
	Serial.begin(115200);
}

void loop() {
	std::stringstream ss;
	ss << 12356 << "hogehoge" << '\n';

	Serial.println(ss.str().c_str());

	delay(10);
}
