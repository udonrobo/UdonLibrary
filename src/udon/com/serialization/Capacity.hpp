//-----------------------------------------------
//
//	UdonLibrary
// 
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  シリアライズ関係
//
//-----------------------------------------------


#pragma once

#include <limits.h>

#include <udon/stl/EnableSTL.hpp>

#include <type_traits>
#include <utility>

#include <udon/algorithm/CRC8.hpp>
#include <udon/math/Ceil.hpp>
#include <udon/utility/Parsable.hpp>
#include <udon/types/Float.hpp>


namespace udon
{

    /// @brief シリアライズ後のビット数を求める
    /// @tparam Bool bool型
    /// @param
    /// @return
    template <typename Bool>
    inline constexpr auto Capacity(const Bool&)
        -> typename std::enable_if<std::is_same<Bool, bool>::value, size_t>::type
    {
        return 1;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam Integer 整数型
    /// @param
    /// @return
    template <typename Integer>
    inline constexpr auto Capacity(const Integer&)
        -> typename std::enable_if<std::is_integral<Integer>::value && not std::is_same<Integer, bool>::value, size_t>::type
    {
        return sizeof(Integer) * CHAR_BIT;
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam T 浮動小数点型
    /// @param
    /// @return
    template <typename Floating>
    inline constexpr auto Capacity(const Floating&)
        -> typename std::enable_if<std::is_floating_point<Floating>::value, size_t>::type
    {
        return sizeof(udon::float32_t) * CHAR_BIT;
    }

    UDON_HAS_MEMBER_FUNCTION(capacity);           // udon::has_member_function_capacity<T>
    UDON_HAS_STATIC_MEMBER_FUNCTION(Capacity);    // udon::has_static_member_function_Capacity<T>

    /// @brief シリアライズ後のビット数を求める
    /// @tparam T capacity メンバ関数を持つ型
    /// @param obj
    /// @return
    template <typename T>
    inline constexpr auto Capacity(const T& obj)
        -> typename std::enable_if<udon::has_member_function_capacity<T>::value, size_t>::type
    {
        return const_cast<T&>(obj).capacity();    // T::capacity() が const でないメンバ関数な場合、obj から呼び出しできないためconstを外す
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam T  Capacity 静的メンバ関数を持つ型
    /// @param obj
    /// @return
    template <typename T>
    inline constexpr auto Capacity(const T&)
        -> typename std::enable_if<udon::has_static_member_function_Capacity<T>::value, size_t>::type
    {
        return T::Capacity();
    }

    /// @brief シリアライズ後のビット数を求める
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
        return Capacity<typename std::remove_reference<T>::type>(arg);
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam T
    /// @param arg 可変長引数
    /// @return
    template <typename T, typename... Args>
    inline constexpr size_t Capacity(T&& arg, Args&&... args)
    {
        return Capacity(arg) + Capacity(std::forward<Args>(args)...);
    }

    /// @brief シリアライズ後のビット数を求める
    /// @tparam T
    /// @return
    template <typename T>
    inline constexpr size_t Capacity()
    {
        return Capacity(T{});
    }

    /// @brief チェックサムを含めたシリアライズ後のバイト数を求める
    /// @tparam T
    /// @param obj
    /// @return
    template <typename T>
    inline constexpr size_t CapacityWithChecksum(const T& obj)
    {
        return udon::Ceil(Capacity(obj) / static_cast<double>(CHAR_BIT)) +
               udon::CRC8_SIZE;
    }

    /// @brief チェックサムを含めたシリアライズ後のバイト数を求める
    /// @tparam T
    /// @return
    template <typename T>
    inline constexpr size_t CapacityWithChecksum()
    {
        return udon::Ceil(Capacity(T{}) / static_cast<double>(CHAR_BIT)) +
               udon::CRC8_SIZE;
    }

}    // namespace udon
