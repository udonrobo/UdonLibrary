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
//    二次元ベクトル
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Types/Forward/PolarFwd.hpp>
#include <Udon/Types/Forward/Vector2DFwd.hpp>
#include <Udon/Types/Forward/Vector3DFwd.hpp>
#include <Udon/Types/Forward/Vector4DFwd.hpp>


template <typename T>
inline Udon::Vector3D<T> Udon::Vector2D<T>::xy0() const noexcept
{
    return { x, y, 0 };
}

template <typename T>
inline Udon::Vector4D<T> Udon::Vector2D<T>::xy00() const noexcept
{
    return { x, y, 0, 0 };
}

template <typename T>
inline Udon::Polar_impl<T> Udon::Vector2D<T>::toPolar() const noexcept
{
    return { length(), angle() };
}

namespace Udon
{
    using Vec2 = Vector2D<double>;
}
