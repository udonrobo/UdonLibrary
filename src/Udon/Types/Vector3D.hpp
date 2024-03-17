//
//    三次元ベクトル
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "Forward/Vector2D.hpp"
#include "Forward/Vector3D.hpp"

inline Udon::Vec2 Udon::Vec3::xy() const noexcept
{
    return { x, y };
}


namespace Udon
{
    using Vec3 = Vec3;
}
