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

#include <Udon/Types/Forward/EulerFwd.hpp>
#include <Udon/Types/Forward/QuaternionFwd.hpp>

template <typename T>
Udon::Euler3D<T> Udon::Quaternion::toEuler() const noexcept
{
    return {
        toRoll(), 
        toPitch(),
        toYaw(),  
    };
}
