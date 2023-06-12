#pragma once

#include <udon/types/EulerFwd.hpp>
#include <udon/types/QuaternionFwd.hpp>

template <typename T>
udon::Euler3D<T> udon::Quaternion::toEuler() const noexcept
{
    return {
        std::atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y)), /* roll  */
        std::asin(2 * (w * y - z * x)),                           /* pitch */
        std::atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z)), /* yaw   */
    };
}
