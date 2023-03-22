#pragma once

#ifdef _WIN64
asd
#endif

namespace udon
{

#if __has_include(<vector>) && !defined(UDON_HAS_STL)
#	define UDON_HAS_STL
#endif

/// @brief float point
#ifdef _MSC_VER
#	ifdef _WIN64
#		define UDON_HAS_FLOAT32
#		define UDON_HAS_FLOAT64
		using float32_t = float;
		using float64_t = double;
#	elif _WIN32
#		define UDON_HAS_FLOAT32
		using float32_t = double;
#	endif
#elif __GNUC__
#	if __SIZEOF_DOUBLE__ == 8 && __SIZEOF_FLOAT__ == 4
#		define UDON_HAS_FLOAT32
#		define UDON_HAS_FLOAT64
		using float32_t = float;
		using float64_t = double;
#	elif __SIZEOF_DOUBLE__ == 4 && __SIZEOF_FLOAT__ == 4
#		define UDON_HAS_FLOAT32
		using float32_t = double;
#	endif
#endif

}