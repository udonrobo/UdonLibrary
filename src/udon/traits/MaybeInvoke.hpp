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
//  メンバ関数があれば呼び出す黒魔術関数
//
//-----------------------------------------------


#pragma once

#include <udon/traits/HasMember.hpp>

/// @brief インスタンスに特定のメンバ関数があれば呼び出す。存在しなければ何もしない。
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

    /// @brief udon::MaybeInvoke_begin

#ifndef UDON_HAS_MEMBER_FUNCTION_BEGIN
#    define UDON_HAS_MEMBER_FUNCTION_BEGIN
    UDON_HAS_MEMBER_FUNCTION(begin);
#endif
#ifndef UDON_MAYBE_INVOKE_BEGIN
#    define UDON_MAYBE_INVOKE_BEGIN
    UDON_MAYBE_INVOKE(begin);
#endif

    /// @brief udon::MaybeInvoke_update

#ifndef UDON_HAS_MEMBER_FUNCTION_UPDATE
#    define UDON_HAS_MEMBER_FUNCTION_UPDATE
    UDON_HAS_MEMBER_FUNCTION(update);
#endif
#ifndef UDON_MAYBE_INVOKE_UPDATE
#    define UDON_MAYBE_INVOKE_UPDATE
    UDON_MAYBE_INVOKE(update);
#endif

    /// @brief udon::MaybeInvoke_show

#ifndef UDON_HAS_MEMBER_FUNCTION_SHOW
#    define UDON_HAS_MEMBER_FUNCTION_SHOW
    UDON_HAS_MEMBER_FUNCTION(show);
#endif
#ifndef UDON_MAYBE_INVOKE_SHOW
#    define UDON_MAYBE_INVOKE_SHOW
    UDON_MAYBE_INVOKE(show);
#endif

    /// @brief udon::MaybeInvoke_showRaw

#ifndef UDON_HAS_MEMBER_FUNCTION_SHOW_RAW
#    define UDON_HAS_MEMBER_FUNCTION_SHOW_RAW
    UDON_HAS_MEMBER_FUNCTION(showRaw);
#endif
#ifndef UDON_MAYBE_INVOKE_SHOW_RAW
#    define UDON_MAYBE_INVOKE_SHOW_RAW
    UDON_MAYBE_INVOKE(showRaw);
#endif

}    // namespace udon
