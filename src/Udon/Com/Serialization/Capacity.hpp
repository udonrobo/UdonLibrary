//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    シリアライズ関係
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>
#include <utility>
#include <limits.h>

#include <Udon/Math/Math.hpp>
#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Utility/Parsable.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Utility/Concept.hpp>
#include <Udon/Com/Serialization/Traits.hpp>

#if CHAR_BIT != 8
#    error "1byte is must be 8bit"
#endif

namespace Udon
{

    /// @brief シリアライズ後のビット数を求める
    /// @tparam Bool bool型
    UDON_CONCEPT_BOOL
    inline constexpr size_t Capacity(Bool)
    {
        return 1;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam IntegralNotBool 整数型かつbool型でない型
    UDON_CONCEPT_INTEGRAL_NOT_BOOL
    inline constexpr size_t Capacity(IntegralNotBool)
    {
        return sizeof(IntegralNotBool) * CHAR_BIT;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam FloatingPoint 浮動小数点型
    UDON_CONCEPT_FLOATING_POINT
    inline constexpr size_t Capacity(FloatingPoint)
    {
        return sizeof(Udon::float32_t) * CHAR_BIT;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam Array 配列型
    UDON_CONCEPT_ARRAY
    inline constexpr size_t Capacity(const Array& obj)
    {
        return Capacity(*obj) * std::extent<Array>::value;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam Capacitive capacity メソッドを持つ型
    template <typename Capacitive, typename std::enable_if<Udon::Details::HasMemberFunctionCapacity<Capacitive>::value, std::nullptr_t>::type* = nullptr>
    inline constexpr size_t Capacity(const Capacitive& obj)
    {
        return obj.capacity();
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam CapacityCallable Capacity 関数から呼び出し可能な型
    template <typename CapacityCallable, typename std::enable_if<Udon::Details::CapacityCallable<CapacityCallable>::value, std::nullptr_t>::type* = nullptr>
    inline constexpr size_t Capacity(const CapacityCallable& obj)
    {
        return ::Capacity(obj);
    }

    /// @brief 可変長引数展開用関数 最終呼び出し
    inline constexpr size_t Capacity()
    {
        return 0;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @remark 無限再帰呼び出しを防ぐために引数が二つ以上必要になっています。
    /// @param first  可変長引数 最初の引数
    /// @param second 可変長引数 二番目の引数
    /// @param args   可変長引数 三番目以降の引数
    /// @return シリアライズ後のビット数
    template <typename First, typename Second, typename... Args>
    inline constexpr size_t Capacity(const First& first, const Second& second, const Args&... args)
    {
        return Capacity(first) + Capacity(second) + Capacity(args...);
    }

    /// @brief チェックサムを含めたシリアライズ後のバイト数を求める
    /// @tparam T
    /// @return
    template <typename T>
    inline constexpr size_t CapacityWithChecksum()
    {
        static_assert(std::is_arithmetic<T>::value or Details::HasMemberFunctionCapacity<T>::value or Details::CapacityCallable<T>::value, "Capacity is not defined.");

        return Udon::Ceil(Capacity(T{}) / static_cast<double>(CHAR_BIT)) + Udon::CRC8_SIZE;
    }

}    // namespace Udon
