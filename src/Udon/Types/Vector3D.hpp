//
//    三次元ベクトル
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "Forward/Vector2D.hpp"
#include "Forward/Vector3D.hpp"

inline Udon::Vector2D Udon::Vector3D::xy() const noexcept
{
    return { x, y };
}


namespace Udon
{
    using Vec3 = Vector3D;
}
