//
//    四次元ベクトル
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "Forward/Vector2D.hpp"
#include "Forward/Vector3D.hpp"
#include "Forward/Vector4D.hpp"

inline Udon::Vector2D Udon::Vector4D::xy() const
{
    return { x, y };
}

inline Udon::Vector3D Udon::Vector4D::xyz() const
{
    return { x, y, z };
}

namespace Udon
{
    using Vec4 = Vector4D;
}
