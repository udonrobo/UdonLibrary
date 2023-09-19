//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    オイラー角
//
//-------------------------------------------------------------------


#pragma once

#include "Forward/EulerFwd.hpp"
#include "Forward/QuaternionFwd.hpp"

template <typename T>
Udon::Quaternion Udon::Euler3D<T>::toQuaternion() const noexcept
{
    const auto cr = cos(static_cast<double>(roll ) / 2);
    const auto cp = cos(static_cast<double>(pitch) / 2);
    const auto cy = cos(static_cast<double>(yaw  ) / 2);
    const auto sr = sin(static_cast<double>(roll ) / 2);
    const auto sp = sin(static_cast<double>(pitch) / 2);
    const auto sy = sin(static_cast<double>(yaw  ) / 2);
    return {
        cy * cp * sr - sy * sp * cr, /*x*/
        sy * cp * sr + cy * sp * cr, /*y*/
        sy * cp * cr - cy * sp * sr, /*z*/
        cy * cp * cr + sy * sp * sr, /*w*/
    };
}
