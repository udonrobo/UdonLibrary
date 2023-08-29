#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>
#include <stddef.h>

#include "VoidT.hpp"

namespace Udon
{

    namespace Traits
    {

        /// @brief T に `size_t T::capacity()` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionCapacity : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionCapacity<T, std::void_t<
                                                typename std::is_same<
                                                    decltype(std::declval<T>().capacity()), size_t>::type>> : std::true_type
        {
        };

        /// @brief グローバル関数 `template <typename T> size_t Capacity(const T&)` が呼び出せるか
        template <typename, typename = void>
        struct CapacityInvocable : std::false_type
        {
        };
        template <typename T>
        struct CapacityInvocable<T, std::void_t<
                                        typename std::is_same<
                                            decltype(Capacity(std::declval<const T&>())), size_t>::type>> : std::true_type
        {
        };

        /// @brief シリアライズ後のバッファのビット数を取得可能である
        /// @remark 容量を取得可能であるには、次の "どちらか一方" の要件を満たす必要がある。
        ///         T に `size_t T::capacity()` 関数が存在する
        ///         グローバル関数 `template <typename T> size_t Capacity(const T&)` が呼び出せる
        template <typename, typename = void>
        struct Capacitable : std::false_type
        {
        };
        template <typename T>
        struct Capacitable<T, typename std::enable_if<
                                  HasMemberFunctionCapacity<T>::value ^ CapacityInvocable<T>::value>::type> : std::true_type
        {
        };

        /// @brief シリアライズ後のバッファのビット数を取得する
        /// @return シリアライズ後のバッファのビット数
        template <typename HasMemberFunctionCapacity, typename std::enable_if<Traits::HasMemberFunctionCapacity<HasMemberFunctionCapacity>::value, std::nullptr_t>::type* = nullptr>
        constexpr size_t InvokeCapacity(HasMemberFunctionCapacity&& rhs)
        {
            return rhs.capacity();
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
