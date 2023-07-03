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

#include <Udon/Traits/HasMember.hpp>
#include <Udon/Com/Serialization/Capacity.hpp>

/// @brief メンバ変数のパースを可能にする
/// @param ... パース可能なメンバー変数(,区切り)
/// @details
/// - パース可能なメンバー変数は、以下の条件を満たす必要がある
///   - メンバー変数の型が、以下のいずれかであること
///     - パース可能な型
///     - パース可能な型の配列
/// - このマクロを使用すると、以下のメンバー関数が定義される
///   - constexpr size_t capacity() const
///     - シリアライズ時に必要なバッファのビットサイズを返す
///   - template <typename Acc> void accessor(Acc& acc)
///     - シリアライズ、デシリアライズ時に使用するアクセッサ
#define UDON_PARSABLE(...)                  \
    using is_parsable_tag = void;           \
    constexpr size_t capacity() const       \
    {                                       \
        return Udon::Capacity(__VA_ARGS__); \
    }                                       \
    template <typename Acc>                 \
    void accessor(Acc& acc)                 \
    {                                       \
        acc(__VA_ARGS__);                   \
    }

namespace Udon
{

#ifndef UDON_HAS_MEMBER_FUNCTION_ACCESSOR
#    define UDON_HAS_MEMBER_FUNCTION_ACCESSOR
    UDON_HAS_MEMBER_ITERATE_FUNCTION(accessor);
#endif

#ifndef UDON_HAS_MEMBER_TYPE_IS_PARSABLE_TAG
#    define UDON_HAS_MEMBER_TYPE_IS_PARSABLE_TAG
    UDON_HAS_MEMBER_TYPE(is_parsable_tag);
#endif

    namespace detail
    {
        template <typename T>
        struct is_parsable_helper
        {
            static constexpr bool value = std::is_arithmetic<T>::value || Udon::has_member_type_is_parsable_tag<T>::value;
        };

        template <typename T>
        struct is_parsable_helper<T[]> : is_parsable_helper<T>
        {
        };

        template <typename T, size_t N>
        struct is_parsable_helper<T[N]> : is_parsable_helper<T>
        {
        };
        
    }

    template <typename T>
    struct is_parsable : detail::is_parsable_helper<T>
    {
    };


}    // namespace Udon