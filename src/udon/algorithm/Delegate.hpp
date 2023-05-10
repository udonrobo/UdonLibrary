#pragma once

#include <algorithm>
#include <type_traits>
#include <udon/stl/EnableSTL.hpp>

/// @brief 単一のメンバ関数を管理するデリゲータ
namespace udon
{

    template <class, class>
    class Delegate;    // 前方宣言

    /// @brief 単一のメンバ関数を管理するデリゲータ
    template <class T, class R, class... Args>
    class Delegate<T, R(Args...)>
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

    template <class T, class R, class... Args>
    T* Delegate<T, R(Args...)>::object = nullptr;

    template <class T, class R, class... Args>
    typename Delegate<T, R(Args...)>::MemberFunctor Delegate<T, R(Args...)>::functionPtr;

}    // namespace udon