#pragma once

#include <udon/traits/HasMember.hpp>

/// @brief インスタンスに特定のメンバ関数があれば呼び出す。なければ何もしない
/// @param MethodName メンバ関数名
/// @remark コンパイル時に呼び出すかどうか判断します
///         呼び出し関数
#define UDON_MAYBE_INVOKE(MethodName)                                                         \
    template <typename T>                                                                     \
    auto MaybeInvoke_##MethodName(T&& instance)                                               \
        ->typename std::enable_if<udon::has_member_function_##MethodName<T>::value>::type     \
    {                                                                                         \
        instance.MethodName();                                                                \
    }                                                                                         \
    template <typename T>                                                                     \
    auto MaybeInvoke_##MethodName(T&&)                                                        \
        ->typename std::enable_if<not udon::has_member_function_##MethodName<T>::value>::type \
    {                                                                                         \
    }

namespace udon
{

    UDON_MAYBE_INVOKE(begin);

    UDON_MAYBE_INVOKE(update);

    UDON_MAYBE_INVOKE(show);

}    // namespace udon