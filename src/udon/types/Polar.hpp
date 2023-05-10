#pragma once

#include <udon/types/forward/PolarFwd.hpp>
#include <udon/types/forward/Vector2DFwd.hpp>

template <typename T>
inline udon::Vector2D<T> udon::Polar_impl<T>::toVector() const noexcept
{
    return { x(), y() };
}

namespace udon
{
    using Polar = udon::Polar_impl<double>;
}
