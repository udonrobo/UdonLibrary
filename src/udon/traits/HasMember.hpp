#pragma once

#include <udon/stl/EnableSTL.hpp>

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
    };

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
    };

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
    };

/// @brief メンバ変数を走査する関数の有無を取得する
/// @param A 走査を行うクラスの型
/// @param T 被走査側のクラスの型
/// @remark 生成される trait クラス
///         has_member_iterator_'name'<A, T>::value
#define UDON_HAS_MEMBER_ITERATOR_FUNCTION(name)                                                                     \
    struct has_member_iterator_##name##_impl                                                                        \
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
    struct has_member_iterator_##name : decltype(has_member_iterator_##name##_impl::test<A, T>(0))                  \
    {                                                                                                               \
    };

namespace udon
{

    UDON_HAS_MEMBER_FUNCTION(begin);

    UDON_HAS_MEMBER_FUNCTION(update);

    UDON_HAS_MEMBER_FUNCTION(show);

    UDON_HAS_MEMBER_ITERATOR_FUNCTION(accessor);

}    // namespace udon