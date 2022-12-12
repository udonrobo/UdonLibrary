#include "SerialStream.hpp"

#include <ostream>
#include <sstream>

class Vec2 {
		double x;
		double y;
	public:
		Vec2()
			: x()
			, y()
		{}
		Vec2(double x, double y)
			: x(x)
			, y(y)
		{}
		friend std::ostream& operator<<(std::ostream& os, const Vec2& r) {
			return os << r.x << r.y;
		}
};

void setup() {
	Serial.begin(115200);
}

void loop() {
	Vec2 vec(100, 200);

	std::stringstream ss;
	ss << vec << '\n';
	
	SerialStream::print(ss);

	delay(10);
}
