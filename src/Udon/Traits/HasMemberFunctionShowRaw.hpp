#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

#include "VoidT.hpp"


namespace Udon
{
    namespace Traits
    {
        /// @brief T に `T::showRaw()` 関数が存在するか
        template <typename, typename = void>
        struct HasMemberFunctionShowRaw : std::false_type
        {
        };
        template <typename T>
        struct HasMemberFunctionShowRaw<T, std::void_t<decltype(std::declval<T>().showRaw())>> : std::true_type
        {
        };

        /// @brief T に `T::showRaw() cosnt` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFunctionShowRaw, typename std::enable_if<Traits::HasMemberFunctionShowRaw<HasMemberFunctionShowRaw>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeShowRaw(const HasMemberFunctionShowRaw& rhs)
        {
            rhs.showRaw();
        }

        /// @brief T に `T::showRaw() cosnt` 関数が存在する場合、呼び出す。それ以外の場合、何もしない。
        template <typename HasMemberFunctionShowRaw, typename std::enable_if<not Traits::HasMemberFunctionShowRaw<HasMemberFunctionShowRaw>::value, std::nullptr_t>::type = nullptr>
        void MaybeInvokeShowRaw(const HasMemberFunctionShowRaw&)
        {
            // Do nothing.
        }
    }
}