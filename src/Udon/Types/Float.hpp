//
//    固定ビット長浮動小数点型
//
//    Copyright (c) 2014 Christopher Korman
//    Copyright (c) 2014 John Maddock
//    Copyright (c) 2014 Paul Bristow
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <float.h>

#define BOOST_CSTDFLOAT_MAXIMUM_AVAILABLE_WIDTH 0
#define BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE 0
#define BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE 0
#define BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE 0
#define BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE 0

// Ensure that the compiler has a radix-2 floating-point representation.
#if (!defined(FLT_RADIX) || ((defined(FLT_RADIX) && (FLT_RADIX != 2))))
#    error The compiler does not support any radix-2 floating-point types required for <boost/cstdfloat.hpp>.
#endif

// Check if built-in float is equivalent to Float16, Float32, Float64, float80_t, or Float128.
#if (defined(FLT_MANT_DIG) && defined(FLT_MAX_EXP))
#    if ((FLT_MANT_DIG == 11) && (FLT_MAX_EXP == 16) && (BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE == 0))
#        define BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE float
#        undef BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
#        define BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE 1
#    elif ((FLT_MANT_DIG == 24) && (FLT_MAX_EXP == 128) && (BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
#        define BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE float
#        undef BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
#        define BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE 1
#    elif ((FLT_MANT_DIG == 53) && (FLT_MAX_EXP == 1024) && (BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
#        define BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE float
#        undef BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
#        define BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE 1
#    elif ((FLT_MANT_DIG == 113) && (FLT_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
#        define BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE float
#        undef BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
#        define BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE 1
#    endif
#endif

// Check if built-in double is equivalent to Float16, Float32, Float64, float80_t, or Float128.
#if (defined(DBL_MANT_DIG) && defined(DBL_MAX_EXP))
#    if ((DBL_MANT_DIG == 11) && (DBL_MAX_EXP == 16) && (BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE == 0))
#        define BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE double
#        undef BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
#        define BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE 1
#        define BOOST_FLOAT16_C(x) (x)
#    elif ((DBL_MANT_DIG == 24) && (DBL_MAX_EXP == 128) && (BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
#        define BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE double
#        undef BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
#        define BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE 1
#        define BOOST_FLOAT32_C(x) (x)
#    elif ((DBL_MANT_DIG == 53) && (DBL_MAX_EXP == 1024) && (BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
#        define BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE double
#        undef BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
#        define BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE 1
#        define BOOST_FLOAT64_C(x) (x)
#    elif ((DBL_MANT_DIG == 113) && (DBL_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
#        define BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE double
#        undef BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
#        define BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE 1
#    endif
#endif

// Disable check long double capability even if supported by compiler since some math runtime
// implementations are broken for long double.
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
// Check if built-in long double is equivalent to Float16, Float32, Float64, float80_t, or Float128.
#    if (defined(LDBL_MANT_DIG) && defined(LDBL_MAX_EXP))
#        if ((LDBL_MANT_DIG == 11) && (LDBL_MAX_EXP == 16) && (BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE == 0))
#            define BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE long double
#            undef BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
#            define BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE 1
#        elif ((LDBL_MANT_DIG == 24) && (LDBL_MAX_EXP == 128) && (BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
#            define BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE long double
#            undef BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
#            define BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE 1
#        elif ((LDBL_MANT_DIG == 53) && (LDBL_MAX_EXP == 1024) && (BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
#            define BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE long double
#            undef BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
#            define BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE 1
#        elif ((LDBL_MANT_DIG == 113) && (LDBL_MAX_EXP == 16384) && (BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
#            define BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE long double
#            undef BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
#            define BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE 1
#        endif
#    endif
#endif

namespace Udon
{

    //  定義されている可能性のある型は以下の通りです(処理系によって異なります)
    //  Udon::Float16
    //  Udon::Float32
    //  Udon::Float64
    //  Udon::Float128

#if BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
    using Float16 = BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE;
    static_assert(sizeof(Float16) == 2, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
#    undef BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE
#    define UDON_HAS_FLOAT16
#endif

#if BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
    using Float32 = BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE;
    static_assert(sizeof(Float32) == 4, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
#    undef BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE
#    define UDON_HAS_FLOAT32
#endif

#if BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
    using Float64 = BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE;
    static_assert(sizeof(Float64) == 8, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
#    undef BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE
#    define UDON_HAS_FLOAT64
#endif

#if BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE
    using Float128 = BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE;
    static_assert(sizeof(Float128) == 16, "Sorry, it is a typedef bug. I would appreciate it if you could report the error to Issue");
#    undef BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE
#    define UDON_HAS_FLOAT128
#endif

}    // namespace Udon

#undef BOOST_CSTDFLOAT_MAXIMUM_AVAILABLE_WIDTH
#undef BOOST_CSTDFLOAT_HAS_FLOAT16_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_HAS_FLOAT32_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_HAS_FLOAT64_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_HAS_FLOAT128_NATIVE_TYPE

#undef BOOST_CSTDFLOAT_FLOAT16_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_FLOAT32_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_FLOAT64_NATIVE_TYPE
#undef BOOST_CSTDFLOAT_FLOAT128_NATIVE_TYPE

#undef BOOST_CSTDFLOAT_HAS_FLOAT16
#undef BOOST_CSTDFLOAT_HAS_FLOAT32
