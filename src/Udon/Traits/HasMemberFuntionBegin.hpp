#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{
    namespace Traits
    {
        /// @brief T に `T::begin()` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFuntionBegin : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFuntionBegin<T, std::void_t<decltype(std::declval<T>().begin())>> : std::true_type
        {
        };

        /// @brief T に `T::begin()` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFuntionBegin, typename std::enable_if<Traits::HasMemberFuntionBegin<HasMemberFuntionBegin>::value, std::nullptr_t>::type* = nullptr>
        void MaybeInvokeShow(HasMemberFuntionBegin& rhs)
        {
            rhs.begin();
        }

        /// @brief T に `T::begin()` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFuntionBegin, typename std::enable_if<not Traits::HasMemberFuntionBegin<HasMemberFuntionBegin>::value, std::nullptr_t>::type* = nullptr>
        void MaybeInvokeShow(HasMemberFuntionBegin&)
        {
            // Do nothing.
        }
    }    // namespace Traits
}    // namespace Udon