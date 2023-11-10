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
//    直交座標系
//
//-------------------------------------------------------------------

#pragma once

#include "Forward/Polar.hpp"
#include "Forward/Vector2D.hpp"

inline Udon::Vector2D Udon::Polar::toVector() const noexcept
{
    return { x(), y() };
}
