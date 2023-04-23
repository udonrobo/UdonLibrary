///////////////////////////////////////////////////////////////////////////////
// Copyright Christopher Kormanyos 2014.
// Copyright John Maddock 2014.
// Copyright Paul Bristow 2014.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 固定 bit 浮動小数点型を定義する(プラットフォームによっては定義されない型もあります)
//
// 定義される型: udon::float16_t, udon::float32_t, udon::float64_t, udon::float80_t, udon::float128_t
//

#pragma once

#include <float.h>

#define BOOST_CSTDFLOAT_MAXIMUM_AVAILABLE_WIDTH  0
#define BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE  0
#define BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE  0
#define BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE  0
#define BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE  0
#define BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE 0

// Ensure that the compiler has a radix-2 floating-point representation.
#if (!defined(FLT_RADIX) || ((defined(FLT_RADIX) && (FLT_RADIX != 2))))
	#error The compiler does not support any radix-2 floating-point types required for <boost/cstdfloat.hpp>.
#endif

// Check if built-in float is equivalent to float16_t, float32_t, float64_t, float80_t, or float128_t.
#if(defined(FLT_MANT_DIG) && defined(FLT_MAX_EXP))
  #if  ((FLT_MANT_DIG == 11) && (FLT_MAX_EXP == 16) && (BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE float
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE  1
  #elif((FLT_MANT_DIG == 24) && (FLT_MAX_EXP == 128) && (BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE float
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE  1
  #elif((FLT_MANT_DIG == 53) && (FLT_MAX_EXP == 1024) && (BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE float
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE  1
  #elif((FLT_MANT_DIG == 64) && (FLT_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT80_NATIVE_TYPE float
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE  1
  #elif((FLT_MANT_DIG == 113) && (FLT_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE float
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE  1
  #endif
#endif

// Check if built-in double is equivalent to float16_t, float32_t, float64_t, float80_t, or float128_t.
#if(defined(DBL_MANT_DIG) && defined(DBL_MAX_EXP))
  #if  ((DBL_MANT_DIG == 11) && (DBL_MAX_EXP == 16) && (BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE double
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE  1
    #define BOOST_FLOAT16_C(x)  (x)
  #elif((DBL_MANT_DIG == 24) && (DBL_MAX_EXP == 128) && (BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE double
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE  1
    #define BOOST_FLOAT32_C(x)  (x)
  #elif((DBL_MANT_DIG == 53) && (DBL_MAX_EXP == 1024) && (BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE double
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE  1
    #define BOOST_FLOAT64_C(x)  (x)
  #elif((DBL_MANT_DIG == 64) && (DBL_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT80_NATIVE_TYPE double
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE  1
  #elif((DBL_MANT_DIG == 113) && (DBL_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
    #define BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE double
    #undef  BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
    #define BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE  1
  #endif
#endif

// Disable check long double capability even if supported by compiler since some math runtime
// implementations are broken for long double.
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
  // Check if built-in long double is equivalent to float16_t, float32_t, float64_t, float80_t, or float128_t.
  #if(defined(LDBL_MANT_DIG) && defined(LDBL_MAX_EXP))
    #if  ((LDBL_MANT_DIG == 11) && (LDBL_MAX_EXP == 16) && (BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE == 0))
      #define BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE long double
      #undef  BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
      #define BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE  1
    #elif((LDBL_MANT_DIG == 24) && (LDBL_MAX_EXP == 128) && (BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
      #define BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE long double
      #undef  BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
      #define BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE  1
    #elif((LDBL_MANT_DIG == 53) && (LDBL_MAX_EXP == 1024) && (BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
      #define BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE long double
      #undef  BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
      #define BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE  1
    #elif((LDBL_MANT_DIG == 64) && (LDBL_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE == 0))
      #define BOOST_CSTDFLOAT_FLOAT80_NATIVE_TYPE long double
      #undef  BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE
      #define BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE  1
    #elif((LDBL_MANT_DIG == 113) && (LDBL_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
      #define BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE long double
      #undef  BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
      #define BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE  1
    #endif
  #endif
#endif


namespace udon
{

	#if BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
	using float16_t = BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE;
	static_assert(sizeof(float16_t) == 2, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
	#undef BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE
	#define UDON_HAS_FLOAT16
	#endif

	#if BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
	using float32_t = BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE;
	static_assert(sizeof(float32_t) == 4, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
	#undef BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE
	#define UDON_HAS_FLOAT32
	#endif

	#if BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
	using float64_t = BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE;
	static_assert(sizeof(float64_t) == 8, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
	#undef BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE
	#define UDON_HAS_FLOAT64
	#endif

	#if BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE
	using float80_t = BOOST_CSTDFLOAT_FLOAT80_NATIVE_TYPE;
	static_assert(sizeof(float80_t) == 10, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
	#undef BOOST_CSTDFLOAT_FLOAT80_NATIVE_TYPE
	#define UDON_HAS_FLOAT80
	#endif

	#if BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
	using float128_t = BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE;
	static_assert(sizeof(float128_t) == 16, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
	#undef BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE
	#define UDON_HAS_FLOAT128
	#endif

}


#undef BOOST_CSTDFLOAT_MAXIMUM_AVAILABLE_WIDTH
#undef BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_HAS_FLOAT80_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
