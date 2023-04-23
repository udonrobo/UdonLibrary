#pragma once

#include <udon/types/forward/Vector2DFwd.hpp>
#include <udon/types/forward/Vector3DFwd.hpp>
#include <udon/types/forward/Vector4DFwd.hpp>

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
