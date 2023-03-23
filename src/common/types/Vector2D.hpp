#pragma once

#include <common\types\forward\Vector2Dfwd.hpp>
#include <common\types\forward\Vector3Dfwd.hpp>
#include <common\types\forward\Vector4Dfwd.hpp>

template<class Ty>
inline udon::Vector3D<Ty> udon::Vector2D<Ty>::xy0() const
{
    return { x, y, 0 };
}

template<class Ty>
inline udon::Vector4D<Ty> udon::Vector2D<Ty>::xy00() const
{
    return { x, y, 0, 0 };
}


namespace udon
{
    using Vec2 = Vector2D<double>;
}