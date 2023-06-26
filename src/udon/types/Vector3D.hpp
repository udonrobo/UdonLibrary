//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  三次元ベクトル
//
//-----------------------------------------------


#pragma once

#include <udon/types/Vector2DFwd.hpp>
#include <udon/types/Vector3DFwd.hpp>
#include <udon/types/Vector4DFwd.hpp>

template <class Ty>
inline udon::Vector2D<Ty> udon::Vector3D<Ty>::xy() const noexcept
{
    return { x, y };
}

template <class Ty>
inline udon::Vector4D<Ty> udon::Vector3D<Ty>::xyz0() const noexcept
{
    return { x, y, z, 0 };
}

namespace udon
{
    using Vec3 = Vector3D<double>;
}
