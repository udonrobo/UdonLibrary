//
//    二次元ベクトル
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include "Forward/Polar.hpp"
#include "Forward/Vector2D.hpp"
#include "Forward/Vector3D.hpp"

inline Udon::Vec3 Udon::Vec2::xy0() const noexcept
{
    return { x, y, 0 };
}

inline Udon::Polar Udon::Vec2::toPolar() const noexcept
{
    return { length(), angle() };
}
