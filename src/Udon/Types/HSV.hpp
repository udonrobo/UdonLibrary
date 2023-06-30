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
//    HSV色空間
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Types/HSVFwd.hpp>
#include <Udon/Types/RGBFwd.hpp>

/// @brief RGB色空間からHSV色空間への変換
/// @return HSV色空間
inline Udon::RGB::RGB(const HSV& hsv) noexcept
    : RGB(hsv.toRGB())
{
}

/// @brief HSV色空間からRGB色空間への変換
/// @return RGB色空間
inline Udon::RGB Udon::HSV::toRGB() const noexcept
{
    const value_type f = h * 6;
    const value_type p = (255 - s) * v >> 8;
    const value_type q = (255 - (f * s >> 8)) * v >> 8;
    const value_type t = (255 - ((255 - f) * s >> 8)) * v >> 8;
    switch (h * 6 >> 8)
    {
    case 0: return { v, t, p };
    case 1: return { q, v, p };
    case 2: return { p, v, t };
    case 3: return { p, q, v };
    case 4: return { t, p, v };
    case 5: return { v, p, q };
    default: return {};
    }
}