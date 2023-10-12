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

#include <Udon/Algorithm/Math.hpp>
#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Traits/Parsable.hpp>
#include <Udon/Traits/Concept.hpp>

#if CHAR_BIT != 8
#    error "1byte is must be 8bit"
#endif

namespace Udon
{

    namespace Detail
    {

        /// @brief bool型
        UDON_CONCEPT_BOOL
        inline constexpr size_t PackedBitSizeImpl(Bool)
        {
            return 1;
        }

        /// @brief 整数型かつbool型でない型
        UDON_CONCEPT_INTEGRAL_NOT_BOOL
        inline constexpr size_t PackedBitSizeImpl(IntegralNotBool)
        {
            return sizeof(IntegralNotBool) * CHAR_BIT;
        }

        /// @brief 浮動小数点型
        UDON_CONCEPT_FLOATING_POINT
        inline constexpr size_t PackedBitSizeImpl(FloatingPoint)
        {
            return sizeof(Udon::float32_t) * CHAR_BIT;
        }

        /// @brief Capacity 関数から呼び出し可能な型
        UDON_CONCEPT_CAPACITABLE
        inline constexpr size_t PackedBitSizeImpl(PackedSizable&& obj)
        {
            return Udon::Traits::InvokeCapacity(std::forward<PackedSizable>(obj));
        }

        /// @brief 列挙型
        UDON_CONCEPT_ENUM
        inline constexpr size_t PackedBitSizeImpl(Enum)
        {
            return sizeof(typename std::underlying_type<Enum>::type) * CHAR_BIT;
        }

        /// @brief 組み込み配列型
        UDON_CONCEPT_ARRAY
        inline constexpr size_t PackedBitSizeImpl(const Array& obj)
        {
            // 各要素の容量 * 配列の要素数
            return PackedBitSizeImpl(*obj) * std::extent<Array>::value;
        }

        /// @brief 可変長引数展開用
        inline constexpr size_t PackedBitSizeArgsUnpack()
        {
            return 0;
        }

        /// @brief 可変長引数展開用
        template <typename T>
        inline constexpr size_t PackedBitSizeArgsUnpack(T&& obj)
        {
            return Detail::PackedBitSizeImpl(std::forward<T>(obj));
        }

        /// @brief 可変長引数展開用
        template <typename Head, typename... Args>
        inline constexpr size_t PackedBitSizeArgsUnpack(Head&& first, Args&&... args)
        {
            return PackedBitSizeArgsUnpack(std::forward<Head>(first)) + PackedBitSizeArgsUnpack(std::forward<Args>(args)...);
        }

    }    // namespace Detail

    /// @brief シリアライズ後のビット数を求める
    /// @tparam Args シリアライズ対象の型
    /// @param args シリアライズ対象の値
    /// @return シリアライズ後のビット数
    template <typename... Args>
    inline constexpr size_t PackedBitSize(Args&&... args)
    {
        return Detail::PackedBitSizeArgsUnpack(std::forward<Args>(args)...);
    }

    /// @brief チェックサムを含めたシリアライズ後のバイト数を求める
    /// @tparam T シリアライズ対象の型
    /// @return シリアライズ後のバイト数
    template <typename T>
    inline constexpr size_t PackedSize()
    {
        static_assert(Udon::Traits::Parsable<T>::value, "T must be parsable type.");
        return Udon::Ceil(PackedBitSize(T{}) / static_cast<double>(CHAR_BIT)) + Udon::CRC8_SIZE;
    }

}    // namespace Udon
