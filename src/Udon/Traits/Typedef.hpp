// C++11 での型定義の簡略化

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{
    namespace Traits
    {

        template <typename T>
        using DecayT = typename std::decay<T>::type;

        template <typename T>
        using RemoveConstT = typename std::remove_const<T>::type;

        template <typename T>
        using RemoveReferenceT = typename std::remove_reference<T>::type;

        template <typename T>
        using IsBool = std::is_same<T, bool>;

        template <typename T>
        using IsIntegral = std::is_integral<T>;

        template <typename T>
        using IsIntegralNotBool = std::integral_constant<bool, IsIntegral<T>::value and not IsBool<T>::value>;

        template <typename T>
        using IsFloatingPoint = std::is_floating_point<T>;

        template <typename T>
        using IsArithmetic = std::is_arithmetic<T>;

        template <typename T>
        using IsScalar = std::is_scalar<T>;

        template <typename T>
        using IsEnum = std::is_enum<T>;

        template <typename T>
        using IsArray = std::is_array<T>;

        template <typename T>
        using IsCString = std::is_same<DecayT<T>, const char*>;

    }    // namespace Traits

    namespace Traits
    {
        // std::enable_if<std::is_same<T, U>::value, std::nullptr_t>::type
        // ↓
        // EnableIfNullptr<std::is_same<T, U>>::type

        /// @brief
        /// @details T::value == true  の場合 type が std::nullptr_t に実体化
        /// @details T::value == false の場合 type は実体化されない
        template <typename T>
        using EnableIfNullptrT = typename std::enable_if<T::value, std::nullptr_t>::type;

        /// @brief
        /// @details T::value == true  の場合 type は実体化されない
        /// @details T::value == false の場合 type が std::nullptr_t に実体化
        template <typename T>
        using DisableIfNullptrT = typename std::enable_if<not T::value, std::nullptr_t>::type;

    }    // namespace Traits
}    // namespace Udon
