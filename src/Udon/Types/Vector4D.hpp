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
//    四次元ベクトル
//
//-------------------------------------------------------------------


#pragma once

#include "Forward/Vector2DFwd.hpp"
#include "Forward/Vector3DFwd.hpp"
#include "Forward/Vector4DFwd.hpp"

template <class Ty>
inline Udon::Vector2D<Ty> Udon::Vector4D<Ty>::xy() const
{
    return { x, y };
}

template <class Ty>
inline Udon::Vector3D<Ty> Udon::Vector4D<Ty>::xyz() const
{
    return { x, y, z };
}

namespace Udon
{
    using Vec4 = Vector4D<double>;
}
