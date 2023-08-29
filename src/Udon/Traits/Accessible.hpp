#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{

    namespace Traits
    {

        namespace Detail
        {
            class DummyAccessor;
        }

        /// @brief T に `template <typename Acc> void T::accessor(Acc&)` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionAccessor : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionAccessor<T, std::void_t<
                                                typename std::is_same<
                                                    decltype(std::declval<T>().accessor(std::declval<Detail::DummyAccessor&>())), void>::type>> : std::true_type
        {
        };

        /// @brief グローバル関数 `template <typename Acc, typename T> void Accessor(Acc&, T&)` が呼び出せるか
        template <typename, typename = void>
        struct AccessorInvocable : std::false_type
        {
        };
        template <typename T>
        struct AccessorInvocable<T, std::void_t<
                                        typename std::is_same<
                                            decltype(Accessor(std::declval<Detail::DummyAccessor&>(), std::declval<T&>())), void>::type>> : std::true_type
        {
        };

        /// @brief メンバ変数列挙用の関数が定義されているか
        /// @remark アクセス可能であるには、次の "どちらか一方" の要件を満たす必要がある。
        ///         T に `template <typename Acc> void T::accessor(Acc&)` 関数が存在する
        ///         グローバル関数 `template <typename Acc, typename T> void Accessor(Acc&, T&)` が呼び出せる
        template <typename, typename = void>
        struct Accessible : std::false_type
        {
        };
        template <typename T>
        struct Accessible<T, typename std::enable_if<
                                 HasMemberFunctionAccessor<T>::value ^ AccessorInvocable<T>::value>::type> : std::true_type
        {
        };

        template <typename Acc, typename HasMemberFunctionAccessor, typename std::enable_if<Traits::HasMemberFunctionAccessor<HasMemberFunctionAccessor>::value, std::nullptr_t>::type* = nullptr>
        void InvokeAccessor(Acc& acc, HasMemberFunctionAccessor& rhs)
        {
            rhs.accessor(acc);
        }

        template <typename Acc, typename AccessorInvocable, typename std::enable_if<Traits::AccessorInvocable<AccessorInvocable>::value, std::nullptr_t>::type* = nullptr>
        void InvokeAccessor(Acc& acc, AccessorInvocable& rhs)
        {
            Accessor(acc, rhs);
        }

    }    // namespace Traits

}    // namespace Udon