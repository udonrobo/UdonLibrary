#pragma once

#include <common\types\forward\Vector2Dfwd.hpp>
#include <common\types\forward\Vector3Dfwd.hpp>
#include <common\types\forward\Vector4Dfwd.hpp>

template<class Ty>
inline udon::Vector2D<Ty> udon::Vector4D<Ty>::xy() const
{
    return { x, y };
}

template<class Ty>
inline udon::Vector3D<Ty> udon::Vector4D<Ty>::xyz() const
{
    return { x, y, z };
}


namespace udon
{
    using Vec4 = Vector4D<double>;
}
