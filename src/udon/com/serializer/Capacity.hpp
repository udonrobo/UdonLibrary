#pragma once

#include <type_traits>

#include <udon/types/Float.hpp>
#include <udon/traits/HasMember.hpp>

namespace udon
{

    /// @brief シリアライズ後のバイト数を求める
    /// @tparam T 整数型
    /// @param
    /// @return
    template <typename T>
    inline constexpr auto Capacity(const T&) -> std::enable_if_t<std::is_integral<T>::value, size_t>
    {
        return sizeof(T);
    }

    /// @brief シリアライズ後のバイト数を求める
    /// @tparam T 浮動小数点型
    /// @param
    /// @return
    template <typename T>
    inline constexpr auto Capacity(const T&) -> std::enable_if_t<std::is_floating_point<T>::value, size_t>
    {
        return sizeof(udon::float32_t);
    }

    UDON_HAS_MEMBER_FUNCTION(capacity);           // udon::has_member_function_capacity_v<T>
    UDON_HAS_STATIC_MEMBER_FUNCTION(Capacity);    // udon::has_static_member_function_Capacity_v<T>

    /// @brief シリアライズ後のバイト数を求める
    /// @tparam T capacity メンバ関数を持つ型
    /// @param obj
    /// @return
    template <typename T>
    inline constexpr auto Capacity(const T& obj) -> typename std::enable_if<udon::has_member_function_capacity_v<T>, size_t>::type
    {
        return const_cast<T&>(obj).capacity();    // T::capacity() が const でないメンバ関数な場合、obj から呼び出しできないためconstを外す
    }

    /// @brief シリアライズ後のバイト数を求める
    /// @tparam T  Capacity 静的メンバ関数を持つ型
    /// @param obj
    /// @return
    template <typename T>
    inline constexpr auto Capacity(const T&) -> typename std::enable_if<udon::has_static_member_function_Capacity_v<T>, size_t>::type
    {
        return T::Capacity();
    }

    /// @brief シリアライズ後のバイト数を求める
    /// @tparam T 配列の要素の型
    /// @tparam N 配列の要素数
    /// @param obj
    /// @return
    template <typename T, size_t N>
    inline constexpr size_t Capacity(const T (&obj)[N])
    {
        return Capacity(*obj) * N;
    }

    /// @brief 可変長引数展開用関数
    /// @tparam T
    /// @param arg
    /// @return
    template <typename T>
    inline constexpr size_t Capacity(T&& arg)
    {
        return Capacity<std::remove_reference_t<T>>(arg);
    }

    /// @brief シリアライズ後のバイト数を求める
    /// @tparam T
    /// @param arg 可変長引数
    /// @return
    template <typename T, typename... Args>
    inline constexpr std::size_t Capacity(T&& arg, Args&&... args)
    {
        return Capacity(arg) + Capacity(std::forward<Args>(args)...);
    }


}    // namespace udon