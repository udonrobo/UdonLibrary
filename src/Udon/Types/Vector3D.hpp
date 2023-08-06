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

#include <Udon/Types/Forward/Vector2DFwd.hpp>
#include <Udon/Types/Forward/Vector3DFwd.hpp>
#include <Udon/Types/Forward/Vector4DFwd.hpp>

template <class Ty>
inline Udon::Vector2D<Ty> Udon::Vector3D<Ty>::xy() const noexcept
{
    return { x, y };
}

template <class Ty>
inline Udon::Vector4D<Ty> Udon::Vector3D<Ty>::xyz0() const noexcept
{
    return { x, y, z, 0 };
}

namespace Udon
{
    using Vec3 = Vector3D<double>;
}
