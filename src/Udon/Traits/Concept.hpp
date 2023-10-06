#pragma once

#include "Accessible.hpp"
#include "Capacitable.hpp"
#include "HasMemberFunction.hpp"

// bool 型
#define UDON_CONCEPT_BOOL \
    template <typename Bool, typename std::enable_if<std::is_same<Bool, bool>::value, std::nullptr_t>::type = nullptr>

// 整数型
#define UDON_CONCEPT_INTEGRAL \
    template <typename Integral, typename std::enable_if<std::is_integral<Integral>::value, std::nullptr_t>::type = nullptr>

// 整数型であり、かつ bool ではない型
#define UDON_CONCEPT_INTEGRAL_NOT_BOOL \
    template <typename IntegralNotBool, typename std::enable_if<std::is_integral<IntegralNotBool>::value && not std::is_same<IntegralNotBool, bool>::value, std::nullptr_t>::type = nullptr>

// 浮動小数点型
#define UDON_CONCEPT_FLOATING_POINT \
    template <typename FloatingPoint, typename std::enable_if<std::is_floating_point<FloatingPoint>::value, std::nullptr_t>::type = nullptr>

// アトミック型
#define UDON_CONCEPT_ATOMIC \
    template <typename Atomic, typename std::enable_if<std::is_arithmetic<Atomic>::value, std::nullptr_t>::type = nullptr>

// スカラ型
#define UDON_CONCEPT_SCALAR \
    template <typename Scalar, typename std::enable_if<std::is_scalar<Scalar>::value, std::nullptr_t>::type = nullptr>

// 列挙型
#define UDON_CONCEPT_ENUM \
	template <typename Enum, typename std::enable_if<std::is_enum<Enum>::value, std::nullptr_t>::type = nullptr>

// 配列
#define UDON_CONCEPT_ARRAY \
    template <typename Array, typename std::enable_if<std::is_array<Array>::value, std::nullptr_t>::type = nullptr>

// メンバ変数列挙用の関数が定義されている型
#define UDON_CONCEPT_ACCESSIBLE \
    template <typename Accessible, typename std::enable_if<Udon::Traits::Accessible<Accessible>::value, std::nullptr_t>::type = nullptr>

// キャパシティを持つ型
#define UDON_CONCEPT_CAPACITABLE \
    template <typename Capacitable, typename std::enable_if<Udon::Traits::Capacitable<Capacitable>::value, std::nullptr_t>::type = nullptr>

// show() 関数を持つ型
#define UDON_CONCEPT_SHOWABLE \
    template <typename HasMemberFunctionShow, typename std::enable_if<Udon::Traits::HasMemberFunctionShow<HasMemberFunctionShow>::value, std::nullptr_t>::type = nullptr>

// showRaw() 関数を持つ型
#define UDON_CONCEPT_SHOW_RAWABLE \
    template <typename HasMemberFunctionShowRaw, typename std::enable_if<Udon::Traits::HasMemberFunctionShowRaw<HasMemberFunctionShowRaw>::value, std::nullptr_t>::type = nullptr>

// begin() 関数を持つ型
#define UDON_CONCEPT_BEGINNABLE \
    template <typename HasMemberFuntionBegin, typename std::enable_if<Udon::Traits::HasMemberFuntionBegin<Beginnable>::value, std::nullptr_t>::type = nullptr>

// update() 関数を持つ型
#define UDON_CONCEPT_UPDATABLE \
    template <typename HasMemberFunctionUpdate, typename std::enable_if<Udon::Traits::HasMemberFunctionUpdate<HasMemberFunctionUpdate>::value, std::nullptr_t>::type = nullptr>
