#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

#include "VoidT.hpp"


namespace Udon
{
    namespace Traits
    {
        /// @brief T に `T::show()` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionShow : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionShow<T, std::void_t<decltype(std::declval<T>().show())>> : std::true_type
        {
        };

        /// @brief T に `T::show() cosnt` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFunctionShow, typename std::enable_if<Traits::HasMemberFunctionShow<HasMemberFunctionShow>::value, std::nullptr_t>::type* = nullptr>
        void MaybeInvokeShow(const HasMemberFunctionShow& rhs)
        {
            rhs.show();
        }

        /// @brief T に `T::show() cosnt` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFunctionShow, typename std::enable_if<not Traits::HasMemberFunctionShow<HasMemberFunctionShow>::value, std::nullptr_t>::type* = nullptr>
        void MaybeInvokeShow(const HasMemberFunctionShow&)
        {
            // Do nothing.
        }
    }
}