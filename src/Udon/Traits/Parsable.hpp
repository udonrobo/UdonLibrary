//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    メンバ変数のパースを可能にするマクロ
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Traits/Capacitable.hpp>
#include <Udon/Traits/Accessible.hpp>
#include <Udon/Com/Serialization/Capacity.hpp>

namespace Udon
{

    namespace Traits
    {

        /// @brief T がパース可能であるか
        template <typename, typename = void>
        struct Parsable : std::false_type
        {
        };

        template <typename T>
        struct Parsable<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> : std::true_type    // 算術型
        {
        };

        template <typename T>
        struct Parsable<T, typename std::enable_if<Traits::Capacitable<T>::value && Traits::Accessible<T>::value && T().parsable()>::type> : std::true_type    // ユーザー定義型かつパース可能(UDON_PARSABLE()で定義されたメンバー関数を持つ)
        {
        };

        template <typename T, size_t N>
        struct Parsable<T[N]> : Parsable<T>    // 配列
        {
        };

        /// @brief メンバ変数がパース可能であることをコンパイル時に検証する
        template <typename Head>
        inline constexpr bool IsMemberParsable(const Head&)
        {
            return Parsable<typename std::remove_reference<Head>::type>::value;
        }
        template <typename Head, typename... Tails>
        inline constexpr bool IsMemberParsable(const Head& head, const Tails&... tails)
        {
            return IsMemberParsable(head) && IsMemberParsable(tails...);
        }

    }    // namespace Traits

}    // namespace Udon

/// @brief メンバ変数のパースを可能にする
/// @param ... パース可能なメンバー変数(,区切り)
/// @Details
/// - パース可能なメンバー変数は、以下の条件を満たす必要がある
///   - メンバー変数の型が、以下のいずれかであること
///     - パース可能な型
///     - パース可能な型の配列
/// - このマクロを使用すると、以下のメンバー関数が定義される
///   - constexpr size_t capacity() const
///     - シリアライズ時に必要なバッファのビットサイズを返す
///   - template <typename Acc> void accessor(Acc& acc)
///     - シリアライズ、デシリアライズ時に使用するアクセッサ
#define UDON_PARSABLE(...)                                  \
    constexpr bool parsable() const                         \
    {                                                       \
        return Udon::Traits::IsMemberParsable(__VA_ARGS__); \
    }                                                       \
    constexpr size_t capacity() const                       \
    {                                                       \
        return Udon::CapacityBits(__VA_ARGS__);             \
    }                                                       \
    template <typename Acc>                                 \
    void accessor(Acc& acc)                                 \
    {                                                       \
        acc(__VA_ARGS__);                                   \
    }
