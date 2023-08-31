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

        /// @brief T に `size_t T::capacityBits()` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionCapacityBits
            : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionCapacityBits<T, typename std::enable_if<std::is_same<
                                                decltype(std::declval<T>().capacityBits()), size_t>::value>::type>
            : std::true_type
        {
        };

        /// @brief グローバル関数 `template <typename T> size_t Capacity(const T&)` が呼び出せるか
        template <typename, typename = void>
        struct CapacityInvocable
            : std::false_type
        {
        };
        template <typename T>
        struct CapacityInvocable<T, typename std::enable_if<std::is_same<
                                        decltype(Capacity(std::declval<const T&>())), size_t>::value>::type>
            : std::true_type
        {
        };

        /// @brief シリアライズ後のバッファのビット数を取得可能である
        /// @remark 容量を取得可能であるには、次の "どちらか一方" の要件を満たす必要がある。
        ///         T に `size_t T::capacityBits()` 関数が存在する
        ///         グローバル関数 `template <typename T> size_t Capacity(const T&)` が呼び出せる
        template <typename, typename = void>
        struct Capacitable
            : std::false_type
        {
        };
        template <typename T>
        struct Capacitable<T, typename std::enable_if<
                                  HasMemberFunctionCapacityBits<T>::value ^ CapacityInvocable<T>::value>::type>
            : std::true_type
        {
        };
        template <typename T>
        struct Capacitable<T, typename std::enable_if<
                                  HasMemberFunctionCapacityBits<T>::value && CapacityInvocable<T>::value>::type>
            : std::false_type
        {
            static_assert(AlwaysFalse<T>::value, "T has both member function capacityBits() and global function Capacity().");    // メンバ関数とグローバル関数の両方が定義されている場合はコンパイルエラー
        };

        /// @brief シリアライズ後のバッファのビット数を取得する
        /// @return シリアライズ後のバッファのビット数
        template <typename HasMemberFunctionCapacityBits, typename std::enable_if<Traits::HasMemberFunctionCapacityBits<HasMemberFunctionCapacityBits>::value, std::nullptr_t>::type* = nullptr>
        constexpr size_t InvokeCapacity(HasMemberFunctionCapacityBits&& rhs)
        {
            return rhs.capacityBits();
        }

        /// @brief シリアライズ後のバッファのビット数を取得する
        /// @return シリアライズ後のバッファのビット数
        template <typename CapacityInvocable, typename std::enable_if<Traits::CapacityInvocable<CapacityInvocable>::value, std::nullptr_t>::type* = nullptr>
        constexpr size_t InvokeCapacity(CapacityInvocable&& rhs)
        {
            return Capacity(rhs);
        }

    }    // namespace Traits

}    // namespace Udon
