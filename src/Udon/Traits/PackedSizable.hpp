#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>
#include <stddef.h>

#include "VoidT.hpp"
#include "AlwaysFalse.hpp"

namespace Udon
{

    namespace Traits
    {

        /// @brief T に `size_t T::packedBitSize()` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionPackedBitSize
            : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionPackedBitSize<T, typename std::enable_if<std::is_same<
                                                decltype(std::declval<T>().packedBitSize()), size_t>::value>::type>
            : std::true_type
        {
        };

        /// @brief グローバル関数 `template <typename T> size_t Capacity(const T&)` が呼び出せるか
        template <typename, typename = void>
        struct PackedSizeInvocable
            : std::false_type
        {
        };
        template <typename T>
        struct PackedSizeInvocable<T, typename std::enable_if<std::is_same<
                                        decltype(PackedSize(std::declval<const T&>())), size_t>::value>::type>
            : std::true_type
        {
        };

        /// @brief シリアライズ後のバッファのビット数を取得可能である
        /// @remark 容量を取得可能であるには、次の "どちらか一方" の要件を満たす必要がある。
        ///         T に `size_t T::packedBitSize()` 関数が存在する
        ///         グローバル関数 `template <typename T> size_t Capacity(const T&)` が呼び出せる
        template <typename, typename = void>
        struct PackedSizable
            : std::false_type
        {
        };
        template <typename T>
        struct PackedSizable<T, typename std::enable_if<
                                  HasMemberFunctionPackedBitSize<T>::value ^ PackedSizeInvocable<T>::value>::type>
            : std::true_type
        {
        };
        template <typename T>
        struct PackedSizable<T, typename std::enable_if<
                                  HasMemberFunctionPackedBitSize<T>::value && PackedSizeInvocable<T>::value>::type>
            : std::false_type
        {
            static_assert(AlwaysFalse<T>::value, "T has both member function packedBitSize() and global function Capacity().");    // メンバ関数とグローバル関数の両方が定義されている場合はコンパイルエラー
        };

        /// @brief シリアライズ後のバッファのビット数を取得する
        /// @return シリアライズ後のバッファのビット数
        template <typename HasMemberFunctionPackedBitSize, typename std::enable_if<Traits::HasMemberFunctionPackedBitSize<HasMemberFunctionPackedBitSize>::value, std::nullptr_t>::type = nullptr>
        constexpr size_t InvokeCapacity(HasMemberFunctionPackedBitSize&& rhs)
        {
            return rhs.packedBitSize();
        }

        /// @brief シリアライズ後のバッファのビット数を取得する
        /// @return シリアライズ後のバッファのビット数
        template <typename PackedSizeInvocable, typename std::enable_if<Traits::PackedSizeInvocable<PackedSizeInvocable>::value, std::nullptr_t>::type = nullptr>
        constexpr size_t InvokeCapacity(PackedSizeInvocable&& rhs)
        {
            return PackedSize(rhs);
        }

    }    // namespace Traits

}    // namespace Udon
