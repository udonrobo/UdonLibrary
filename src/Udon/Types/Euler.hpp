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

#include "Forward/Euler.hpp"
#include "Forward/Quaternion.hpp"

inline Udon::Quaternion Udon::Euler::toQuaternion() const noexcept
{
    const auto cr = cos(roll  * 0.5);
    const auto cp = cos(pitch * 0.5);
    const auto cy = cos(yaw   * 0.5);
    const auto sr = sin(roll  * 0.5);
    const auto sp = sin(pitch * 0.5);
    const auto sy = sin(yaw   * 0.5);
    return {
        cy * cp * sr - sy * sp * cr, /*x*/
        sy * cp * sr + cy * sp * cr, /*y*/
        sy * cp * cr - cy * sp * sr, /*z*/
        cy * cp * cr + sy * sp * sr, /*w*/
    };
}
