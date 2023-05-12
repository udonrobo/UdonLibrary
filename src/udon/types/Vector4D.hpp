#pragma once

#include <udon/types/Vector2DFwd.hpp>
#include <udon/types/Vector3DFwd.hpp>
#include <udon/types/Vector4DFwd.hpp>

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
