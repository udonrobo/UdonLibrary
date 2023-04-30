#pragma once

#include <udon/types/forward/Vector2DFwd.hpp>
#include <udon/types/forward/Vector3DFwd.hpp>
#include <udon/types/forward/Vector4DFwd.hpp>

template<class Ty>
inline udon::Vector3D<Ty> udon::Vector2D<Ty>::xy0() const noexcept
{
    return { x, y, 0 };
}

template<class Ty>
inline udon::Vector4D<Ty> udon::Vector2D<Ty>::xy00() const noexcept
{
    return { x, y, 0, 0 };
}


namespace udon
{
    using Vec2 = Vector2D<double>;
}
