//
//    三次元ベクトル
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "Forward/Vector2D.hpp"
#include "Forward/Vector3D.hpp"
#include "Forward/Vector4D.hpp"

inline Udon::Vector2D Udon::Vector3D::xy() const noexcept
{
    return { x, y };
}

inline Udon::Vector4D Udon::Vector3D::xyz0() const noexcept
{
    return { x, y, z, 0 };
}

namespace Udon
{
    using Vec3 = Vector3D;
}
