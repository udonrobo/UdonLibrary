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

namespace Udon
{

    /// @brief シリアライズ後のビット数を求める
    /// @tparam T capacity メンバ関数を持つ型
    /// @param obj
    /// @return
    UDON_CONCEPT_CAPACITIVE
    inline constexpr size_t Capacity(Capacitive&& obj)
    {
        return const_cast<Capacitive&>(obj).capacity();
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam Bool bool型
    /// @param
    /// @return
    UDON_CONCEPT_BOOL
    inline constexpr size_t Capacity(Bool&&)
    {
        return 1;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam IntegerIgnoreBool 整数型
    /// @param
    /// @return
    UDON_CONCEPT_INTEGRAL_NOT_BOOL
    inline constexpr size_t Capacity(IntegralNotBool&&)
    {
        return sizeof(IntegralNotBool) * CHAR_BIT;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam T 浮動小数点型
    /// @param
    /// @return
    UDON_CONCEPT_FLOATING_POINT
    inline constexpr size_t Capacity(FloatingPoint&&)
    {
        return sizeof(Udon::float32_t) * CHAR_BIT;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam T 配列の要素の型
    /// @tparam N 配列の要素数
    /// @param obj
    /// @return
    UDON_CONCEPT_ARRAY
    inline constexpr size_t Capacity(Array&& obj)
    {
        return Capacity(*obj) * (sizeof obj / sizeof *obj);
	}

    /// @brief 可変長引数展開用関数 最終呼び出し
    inline constexpr size_t Capacity()
    {
        return 0;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @param arg 可変長引数
    /// @return シリアライズ後のビット数
    template <typename Head, typename... Args>
    inline constexpr size_t Capacity(Head&& head, Args&&... args)
    {
        return Capacity(std::forward<Head>(head)) + Capacity(std::forward<Args>(args)...);
    }

    /// @brief チェックサムを含めたシリアライズ後のバイト数を求める
    /// @tparam T
    /// @return
    template <typename T>
    inline constexpr size_t CapacityWithChecksum()
    {
        return Udon::Ceil(Capacity(T{}) / static_cast<double>(CHAR_BIT)) + Udon::CRC8_SIZE;
    }

}    // namespace Udon
