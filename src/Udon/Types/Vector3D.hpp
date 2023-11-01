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
//    三次元ベクトル
//
//-------------------------------------------------------------------


#pragma once

#include "Forward/Vector2DFwd.hpp"
#include "Forward/Vector3DFwd.hpp"
#include "Forward/Vector4DFwd.hpp"

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
