#pragma once

#include <type_traits>

/// @brief クラスに含まれる型の有無を取得する
/// @remark 生成される trait クラス
///         has_type_'name'<T>::value
///         has_type_'name'_v<T>
#define UDON_HAS_TYPE(name)                                                         \
template <class T>                                                                  \
class has_type_##name {                                                             \
    template <class U>                                                              \
    static constexpr bool check(typename U::name*)                                  \
    { return true; }                                                                \
    template <class U>                                                              \
    static constexpr bool check(...)                                                \
    { return false; }                                                               \
public:                                                                             \
    static constexpr bool value = check<T>(nullptr);                                \
};                                                                                  \
                                                                                    \
template<class T>                                                                   \
constexpr bool has_type_##name##_v = has_type_##name##<T>::value;


/// @brief 通常メンバ関数の有無を取得する
/// @remark 静的メンバ関数の場合は除きます
///         生成される trait クラス
///         has_member_function_'name'<T>::value
///         has_member_function_'name'_v<T>
#define UDON_HAS_MEMBER_FUNCTION(name)                                                                                            \
struct has_not_static_member_function_##name##_impl {                                                                             \
    template <class T>                                                                                                            \
    static auto check(T&&) -> decltype(T::name(), std::false_type{});                                                             \
    template <class T>                                                                                                            \
    static auto check(...) -> std::true_type;                                                                                     \
};                                                                                                                                \
                                                                                                                                  \
struct has_member_function_##name##_impl{                                                                                         \
	template <class T>                                                                                                            \
    static auto check(T && x) -> decltype(x.name(), has_not_static_member_function_##name##_impl::check<T>(std::declval<T>()));   \
    template <class T>                                                                                                            \
    static auto check(...) -> std::false_type;                                                                                    \
};                                                                                                                                \
template <class T>                                                                                                                \
class has_member_function_##name :                                                                                                \
    public decltype(has_member_function_##name##_impl::check<T>(std::declval<T>())) {};                                           \
                                                                                                                                  \
template <class T>                                                                                                                \
constexpr bool has_member_function_##name##_v = has_member_function_##name##<T>::value;


/// @brief 静的メンバ関数の有無を取得する
/// @remark 通常メンバ関数の場合は除きます
///         生成される trait クラス
///         has_static_member_function_'name'<T>::value
///         has_static_member_function_'name'_v<T>
#define UDON_HAS_STATIC_MEMBER_FUNCTION(name)                                                       \
struct has_static_member_function_##name##_impl {                                                   \
    template <class T>                                                                              \
    static auto check(T&&) -> decltype(T::name(), std::true_type{});                                \
    template <class T>                                                                              \
    static auto check(...) -> std::false_type;                                                      \
};                                                                                                  \
template <class T>                                                                                  \
class has_static_member_function_##name :                                                           \
    public decltype(has_static_member_function_##name##_impl::check<T>(std::declval<T>())) {};      \
                                                                                                    \
template <class T>                                                                                  \
constexpr bool has_static_member_function_##name##_v = has_static_member_function_##name##<T>::value;


#define UDON_HAS_MEMBER_ITERATOR_FUNCTION(name)                                                                                                 \
    template <class A, class T>                                                                                                                 \
    class has_member_iterator_##name                                                                                                            \
    {                                                                                                                                           \
        template<class AA, class TT>                                                                                                            \
        static auto call_##name##(AA& ar, TT& t) -> decltype(t.##name##(ar))                                                                    \
        {                                                                                                                                       \
            return t.##name##(ar);                                                                                                              \
        }                                                                                                                                       \
        template <class TT, class AA>                                                                                                           \
        static auto test(int) -> decltype( call_##name( std::declval<AA&>(), std::declval<TT&>() ), std::true_type{});                          \
        template <class, class>                                                                                                                 \
        static auto test(...) -> std::false_type;                                                                                               \
    public:                                                                                                                                     \
        static const bool value = std::is_same<decltype(test<T, A>(0)), std::true_type>::value;                                                 \
    };                                                                                                                                          \
    template<typename A, typename T>                                                                                                            \
    constexpr bool has_member_iterator_##name##_v = has_member_iterator_##name##<A, T>::value;

namespace udon
{

	UDON_HAS_MEMBER_FUNCTION(update);

	UDON_HAS_MEMBER_FUNCTION(show);

    UDON_HAS_MEMBER_ITERATOR_FUNCTION(accessor);

}
