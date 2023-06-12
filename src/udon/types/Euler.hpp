#pragma once

#include <udon/types/EulerFwd.hpp>
#include <udon/types/QuaternionFwd.hpp>

template <typename T>
udon::Quaternion udon::Euler3D<T>::toQuaternion() const noexcept
{
    const auto cr = std::cos(roll  / 2);
    const auto cp = std::cos(pitch / 2);
    const auto cy = std::cos(yaw   / 2);
    const auto sr = std::sin(roll  / 2);
    const auto sp = std::sin(pitch / 2);
    const auto sy = std::sin(yaw   / 2);
    return {
        cy * cp * sr - sy * sp * cr, /*x*/
        sy * cp * sr + cy * sp * cr, /*y*/
        sy * cp * cr - cy * sp * sr, /*z*/
        cy * cp * cr + sy * sp * sr, /*w*/
    };
}
