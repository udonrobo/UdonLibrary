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
//    RGB色空間
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Types/Forward/RGBFwd.hpp>
#include <Udon/Types/Forward/HSVFwd.hpp>

/// @brief HSV色空間からRGB色空間への変換
/// @param rgb RGB色空間
inline Udon::HSV::HSV(const RGB& rgb) noexcept
    : HSV(rgb.toHSV())
{
}

/// @brief RGB色空間からHSV色空間への変換
/// @return HSV色空間
inline Udon::HSV Udon::RGB::toHSV() const noexcept
{
    const value_type max = std::max({ r, g, b });
    const value_type min = std::min({ r, g, b });
    const value_type d = max - min;
    const value_type s = max == 0 ? 0 : d * 255 / max;
    const value_type v = max;
    if (d == 0)
    {
        return { 0, 0, v };
    }
    const value_type h = [&]() -> value_type
    {
        if (max == r)
        {
            return (g - b) * 42 / d + (g < b ? 255 : 0);
        }
        if (max == g)
        {
            return (b - r) * 42 / d + 85;
        }
        return (r - g) * 42 / d + 170;
    }();
    return { h, s, v };
}
