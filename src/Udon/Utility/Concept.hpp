#pragma once

#include <Udon/Traits/HasMember.hpp>

// bool 型
#define UDON_CONCEPT_BOOL \
    template <typename Bool, typename std::enable_if<std::is_same<Bool, bool>::value>::type* = nullptr>

// 整数型
#define UDON_CONCEPT_INTEGRAL \
    template <typename Integral, typename std::enable_if<std::is_integral<Integral>::value>::type* = nullptr>

// 整数型であり、かつ bool ではない型
#define UDON_CONCEPT_INTEGRAL_NOT_BOOL \
    template <typename IntegralNotBool, typename std::enable_if<std::is_integral<IntegralNotBool>::value && not std::is_same<IntegralNotBool, bool>::value>::type* = nullptr>

// 浮動小数点型
#define UDON_CONCEPT_FLOATING_POINT \
    template <typename FloatingPoint, typename std::enable_if<std::is_floating_point<FloatingPoint>::value>::type* = nullptr>

// 配列
#define UDON_CONCEPT_ARRAY \
    template <typename Array, typename std::enable_if<std::is_array<Array>::value, std::nullptr_t>::type* = nullptr>

