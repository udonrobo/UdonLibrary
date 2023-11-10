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

#include "Forward/Polar.hpp"
#include "Forward/Vector2D.hpp"
#include "Forward/Vector3D.hpp"
#include "Forward/Vector4D.hpp"


inline Udon::Vector3D Udon::Vector2D::xy0() const noexcept
{
    return { x, y, 0 };
}

inline Udon::Vector4D Udon::Vector2D::xy00() const noexcept
{
    return { x, y, 0, 0 };
}

inline Udon::Polar Udon::Vector2D::toPolar() const noexcept
{
    return { length(), angle() };
}

namespace Udon
{
    using Vec2 = Vector2D;
}
