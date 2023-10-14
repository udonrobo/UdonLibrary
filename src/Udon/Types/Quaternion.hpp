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
//    クオータニオン
//
//-------------------------------------------------------------------


#pragma once

#include "Forward/EulerFwd.hpp"
#include "Forward/QuaternionFwd.hpp"

inline Udon::Euler Udon::Quaternion::toEuler() const noexcept
{
    return {
        toRoll(), 
        toPitch(),
        toYaw(),  
    };
}
