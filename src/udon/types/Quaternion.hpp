//-----------------------------------------------
//
//	UdonLibrary
//
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  クオータニオン
//
//-----------------------------------------------


#pragma once

#include <udon/types/EulerFwd.hpp>
#include <udon/types/QuaternionFwd.hpp>

template <typename T>
udon::Euler3D<T> udon::Quaternion::toEuler() const noexcept
{
    return {
        atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y)), /* roll  */
        asin (2 * (w * y - z * x)),                          /* pitch */
        atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z)), /* yaw   */
    };
}
