#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>


namespace Udon
{
    namespace Traits
    {
        /// @brief T に `T::update()` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionUpdate : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionUpdate<T, std::void_t<decltype(std::declval<T>().update())>> : std::true_type
        {
        };

        /// @brief T に `T::update()` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFunctionUpdate, typename std::enable_if<Traits::HasMemberFunctionUpdate<HasMemberFunctionUpdate>::value, std::nullptr_t>::type* = nullptr>
        void MaybeInvokeUpdate(HasMemberFunctionUpdate& rhs)
        {
            rhs.update();
        }

        /// @brief T に `T::update()` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFunctionUpdate, typename std::enable_if<not Traits::HasMemberFunctionUpdate<HasMemberFunctionUpdate>::value, std::nullptr_t>::type* = nullptr>
        void MaybeInvokeUpdate(HasMemberFunctionUpdate& rhs)
        {
            // Do nothing.
        }
    }
}