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
//    メンバ関数有無判定トレイト作成マクロ
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

/// @brief メンバ型名の有無を取得する
/// @remark 生成される trait クラス
///         has_member_type_'name'<T>::value
#define UDON_HAS_MEMBER_TYPE(name)                                                             \
    struct has_member_type_##name##_impl                                                       \
    {                                                                                          \
        template <typename U>                                                                  \
        static auto check(typename U::name*) -> std::true_type;                                \
        template <typename U>                                                                  \
        static auto check(...) -> std::false_type;                                             \
    };                                                                                         \
    template <typename T>                                                                      \
    struct has_member_type_##name : decltype(has_member_type_##name##_impl::check<T>(nullptr)) \
    {                                                                                          \
    }

/// @brief 通常メンバ関数の有無を取得する
/// @remark 静的メンバ関数の場合は除きます
///         引数はvoidである必要があり、戻り値の型は関係ありません
///         生成される trait クラス
///         has_member_function_'name'<T>::value
#define UDON_HAS_MEMBER_FUNCTION(name)                                                                                             \
    struct has_not_static_member_function_##name##_impl                                                                            \
    {                                                                                                                              \
        template <typename T>                                                                                                      \
        static auto check(T&&) -> decltype(T::name(), std::false_type{});                                                          \
        template <typename T>                                                                                                      \
        static auto check(...) -> std::true_type;                                                                                  \
    };                                                                                                                             \
    struct has_member_function_##name##_impl                                                                                       \
    {                                                                                                                              \
        template <typename T>                                                                                                      \
        static auto check(T&& x) -> decltype(x.name(), has_not_static_member_function_##name##_impl::check<T>(std::declval<T>())); \
        template <typename T>                                                                                                      \
        static auto check(...) -> std::false_type;                                                                                 \
    };                                                                                                                             \
    template <typename T>                                                                                                          \
    struct has_member_function_##name : decltype(has_member_function_##name##_impl::check<T>(std::declval<T>()))                   \
    {                                                                                                                              \
    }

/// @brief 静的メンバ関数の有無を取得する
/// @remark 通常メンバ関数の場合は除きます
///         引数はvoidである必要があり、戻り値の型は関係ありません
///         生成される trait クラス
///         has_static_member_function_'name'<T>::value
#define UDON_HAS_STATIC_MEMBER_FUNCTION(name)                                                                                  \
    struct has_static_member_function_##name##_impl                                                                            \
    {                                                                                                                          \
        template <typename T>                                                                                                  \
        static auto check(T&&) -> decltype(T::name(), std::true_type{});                                                       \
        template <typename T>                                                                                                  \
        static auto check(...) -> std::false_type;                                                                             \
    };                                                                                                                         \
    template <typename T>                                                                                                      \
    struct has_static_member_function_##name : decltype(has_static_member_function_##name##_impl::check<T>(std::declval<T>())) \
    {                                                                                                                          \
    }

/// @brief メンバ変数を走査する関数の有無を取得する
/// @param A 走査を行うクラスの型
/// @param T 被走査側のクラスの型
/// @remark 生成される trait クラス
///         has_member_iterate_'name'<A, T>::value
#define UDON_HAS_MEMBER_ITERATE_FUNCTION(name)                                                                      \
    struct has_member_iterate_##name##_impl                                                                         \
    {                                                                                                               \
        template <typename AA, typename TT>                                                                         \
        static auto call_##name(AA& ar, TT& t) -> decltype(t.name(ar))                                              \
        {                                                                                                           \
            return t.name(ar);                                                                                      \
        }                                                                                                           \
        template <typename AA, typename TT>                                                                         \
        static auto test(int) -> decltype(call_##name(std::declval<AA&>(), std::declval<TT&>()), std::true_type{}); \
        template <typename, typename>                                                                               \
        static auto test(...) -> std::false_type;                                                                   \
    };                                                                                                              \
    template <typename A, typename T>                                                                               \
    struct has_member_iterate_##name : decltype(has_member_iterate_##name##_impl::test<A, T>(0))                    \
    {                                                                                                               \
    }

namespace Udon
{

#ifndef UDON_HAS_MEMBER_FUNCTION_BEGIN
#    define UDON_HAS_MEMBER_FUNCTION_BEGIN
    UDON_HAS_MEMBER_FUNCTION(begin);
#endif

#ifndef UDON_HAS_MEMBER_FUNCTION_UPDATE
#    define UDON_HAS_MEMBER_FUNCTION_UPDATE
    UDON_HAS_MEMBER_FUNCTION(update);
#endif

#ifndef UDON_HAS_MEMBER_FUNCTION_SHOW
#    define UDON_HAS_MEMBER_FUNCTION_SHOW
    UDON_HAS_MEMBER_FUNCTION(show);
#endif

}    // namespace Udon