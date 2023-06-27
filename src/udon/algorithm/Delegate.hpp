//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  デリケートクラス
//
//-----------------------------------------------

#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <algorithm>
#include <type_traits>

/// @brief 単一のメンバ関数を管理するデリゲータ
namespace udon
{

    template <class, class, int Unique = 0>
    class Delegate;    // 前方宣言

    /// @brief 単一のメンバ関数を管理するデリゲータ
    /// @tparam T メンバ関数を持つクラス
    /// @tparam R メンバ関数の戻り値の型
    /// @tparam Args メンバ関数の引数の型
    /// @tparam Unique ユニークな値 ※同じクラスの同じ引数、戻り値のメンバ関数を複数登録する場合は、ユニークな値を指定してください。(デフォルト0)
    template <class T, class R, class... Args, int Unique>
    class Delegate<T, R(Args...), Unique>
    {

        using MemberFunctor = R (T::*)(Args...);
        using CFunctor      = R (*)(Args...);

        static T*            object;
        static MemberFunctor functionPtr;

    public:
        Delegate(T* obj, MemberFunctor callback)
        {
            object      = obj;
            functionPtr = callback;
        }

        Delegate(const Delegate& rhs)
        {
            object      = rhs.object;
            functionPtr = rhs.functionPtr;
        }

        template <class Ty = R>
        static auto Execute(Args... args) -> typename std::enable_if<std::is_same<Ty, void>::value, R>::type
        {
            if (object)
            {
                (object->*functionPtr)(std::forward<Args>(args)...);
            }
        }

        template <class Ty = R>
        static auto Execute(Args... args) -> typename std::enable_if<!std::is_same<Ty, void>::value, R>::type
        {
            if (object)
            {
                return (object->*functionPtr)(std::forward<Args>(args)...);
            }
            else
            {
                return {};
            }
        }

        operator CFunctor()
        {
            return Execute;
        }
    };

    template <class T, class R, class... Args, int Unique>
    T* Delegate<T, R(Args...), Unique>::object = nullptr;

    template <class T, class R, class... Args, int Unique>
    typename Delegate<T, R(Args...), Unique>::MemberFunctor Delegate<T, R(Args...), Unique>::functionPtr;

}    // namespace udon