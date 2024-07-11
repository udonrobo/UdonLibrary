//
//    C++11 での型定義の簡略化
//
//    Copyright (c) 2022-2024 udonrobo
//


#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

namespace Udon
{
    namespace Traits
    {

        template <typename T>
        using RemoveConstT = typename std::remove_const<T>::type;

        template <typename T>
        using RemoveVolatileT = typename std::remove_volatile<T>::type;

        template <typename T>
        using RemoveReferenceT = typename std::remove_reference<T>::type;

        template <typename T>
        using RemoveCVRefT = RemoveConstT<RemoveVolatileT<RemoveReferenceT<T>>>;

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
        using DecayT = typename std::decay<T>::type;

        template <typename T>
        using IsCString = std::is_same<DecayT<T>, const char*>;

    }    // namespace Traits

    namespace Traits
    {

        template <bool Test, typename T = void>
        using EnableIfT = typename std::enable_if<Test, T>::type;

        template <bool Test>
        using EnableIfVoidT = EnableIfT<Test, void>;

        template <bool Test, typename T = void>
        using DisableIfT = typename std::enable_if<not Test, T>::type;

        template <bool Test>
        using DisableIfVoidT = DisableIfT<Test, void>;

        /// @brief
        /// @note Test == true  の場合 type が std::nullptr_t に実体化
        /// @note Test == false の場合 type は実体化されない
        template <bool Test>
        using EnableIfNullptrT = EnableIfT<Test, std::nullptr_t>;

        /// @brief
        /// @note Test == true  の場合 type は実体化されない
        /// @note Test == false の場合 type が std::nullptr_t に実体化
        template <bool Test>
        using DisableIfNullptrT = DisableIfT<not Test, std::nullptr_t>;

    }    // namespace Traits
}    // namespace Udon

#ifndef __cpp_lib_void_t
namespace std
{
    template <typename...>
    using void_t = void;
}
#endif
