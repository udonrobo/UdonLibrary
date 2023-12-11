//
//    HSV色空間
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "Forward/HSV.hpp"
#include "Forward/RGB.hpp"

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 26495)
#endif

/// @brief RGB色空間からHSV色空間への変換
/// @return HSV色空間
inline Udon::RGB::RGB(const HSV& hsv) noexcept
    : RGB(hsv.toRGB())
{
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

/// @brief HSV色空間からRGB色空間への変換
/// @return RGB色空間
inline Udon::RGB Udon::HSV::toRGB() const noexcept
{
    const ValueType f = h * 6;
    const ValueType p = (255 - s) * v >> 8;
    const ValueType q = (255 - (f * s >> 8)) * v >> 8;
    const ValueType t = (255 - ((255 - f) * s >> 8)) * v >> 8;
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