#pragma once

// C++ STL を使用できるようにします・

#undef abs

#if !__has_include(<vector>)
#	include <ArduinoSTL.h>
#endif