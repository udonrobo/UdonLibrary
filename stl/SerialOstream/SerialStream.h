#pragma once

#undef min
#undef max

#include <iostream>
#include <sstream>

namespace SerialStream {

inline void print(const std::stringstream& osstm) {
	Serial.print(osstm.str().c_str());
}
	
}
