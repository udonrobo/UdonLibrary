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

#include "Forward/Euler.hpp"
#include "Forward/Quaternion.hpp"

inline Udon::Euler Udon::Quaternion::toEuler() const noexcept
{
    return {
        toRoll(), 
        toPitch(),
        toYaw(),  
    };
}
