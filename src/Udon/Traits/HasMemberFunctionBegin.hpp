#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{
    namespace Traits
    {
        /// @brief T に `T::begin()` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionBegin : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionBegin<T, std::void_t<decltype(std::declval<T>().begin())>> : std::true_type
        {
        };

        /// @brief T に `T::begin()` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFunctionBegin, typename std::enable_if<Traits::HasMemberFunctionBegin<HasMemberFunctionBegin>::value, std::nullptr_t>::type* = nullptr>
        void MaybeInvokeBegin(HasMemberFunctionBegin& rhs)
        {
            rhs.begin();
        }

        /// @brief T に `T::begin()` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFunctionBegin, typename std::enable_if<not Traits::HasMemberFunctionBegin<HasMemberFunctionBegin>::value, std::nullptr_t>::type* = nullptr>
        void MaybeInvokeBegin(HasMemberFunctionBegin&)
        {
            // Do nothing.
        }
    }    // namespace Traits
}    // namespace Udon