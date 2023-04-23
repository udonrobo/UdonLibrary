#pragma once

#include <udon/types/forward/Vector2DFwd.hpp>
#include <udon/types/forward/Vector3DFwd.hpp>
#include <udon/types/forward/Vector4DFwd.hpp>

template<class Ty>
inline udon::Vector2D<Ty> udon::Vector3D<Ty>::xy() const
{
    return { x, y };
}

template<class Ty>
inline udon::Vector4D<Ty> udon::Vector3D<Ty>::xyz0() const
{
    return { x, y, z, 0 };
}


namespace udon
{
    using Vec3 = Vector3D<double>;
}
