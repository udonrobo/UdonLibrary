#pragma once

#include <udon/types/PolarFwd.hpp>
#include <udon/types/Vector2DFwd.hpp>
#include <udon/types/Vector3DFwd.hpp>
#include <udon/types/Vector4DFwd.hpp>


template <typename T>
inline udon::Vector3D<T> udon::Vector2D<T>::xy0() const noexcept
{
    return { x, y, 0 };
}

template <typename T>
inline udon::Vector4D<T> udon::Vector2D<T>::xy00() const noexcept
{
    return { x, y, 0, 0 };
}

template <typename T>
inline udon::Polar_impl<T> udon::Vector2D<T>::toPolar() const noexcept
{
    return { length(), angle() };
}

namespace udon
{
    using Vec2 = Vector2D<double>;
}
