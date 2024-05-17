//
//    RGB色空間
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include "Forward/RGB.hpp"
#include "Forward/HSV.hpp"

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 26495)
#endif

/// @brief HSV色空間からRGB色空間への変換
/// @param rgb RGB色空間
inline Udon::HSV::HSV(const RGB& rgb) noexcept
    : HSV(rgb.toHSV())
{
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

/// @brief RGB色空間からHSV色空間への変換
/// @return HSV色空間
inline Udon::HSV Udon::RGB::toHSV() const noexcept
{
    const ValueType max = std::max({ r, g, b });
    const ValueType min = std::min({ r, g, b });
    const ValueType d   = max - min;
    const ValueType s   = max == 0 ? 0 : d * 255 / max;
    const ValueType v   = max;
    if (d == 0)
    {
        return { 0, 0, v };
    }
    const ValueType h = [&]() -> ValueType
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
