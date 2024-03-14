//
//    直交座標系
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "Forward/Polar.hpp"
#include "Forward/Vector2D.hpp"

inline Udon::Vec2 Udon::Polar::toVector() const noexcept
{
    return { x(), y() };
}
