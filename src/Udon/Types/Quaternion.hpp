//
//    クオータニオン
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

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
