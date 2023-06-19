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
//  数学関数、定数
//
//-----------------------------------------------


#pragma once

#include <math.h>
#include <stdint.h>

namespace udon
{

    constexpr double Pi       = 3.1415926535897932384626433832795;
    constexpr double HalfPi   = 1.5707963267948966192313216916398;
    constexpr double TwoPi    = 6.283185307179586476925286766559;
    constexpr double DegToRad = 0.017453292519943295769236907684886;
    constexpr double RadToDeg = 57.295779513082320876798154814105;

    template <class A, class B>
    inline constexpr A
    Constrain(const A& amt, const B& low, const B& high)
    {
        return fmin(fmax(amt, low), high);
    }

    template <class Ty>
    inline constexpr Ty
    ToRadians(const Ty& rhs)
    {
        return rhs * DegToRad;
    }

    template <class Ty>
    inline constexpr Ty
    ToDegrees(const Ty& rhs)
    {
        return rhs * RadToDeg;
    }

    template <class Ty>
    inline constexpr Ty
    Sq(const Ty& rhs)
    {
        return rhs * rhs;
    }

    template <class Ty>
    inline bool
    BitRead(const Ty& rhs, uint8_t bit)
    {
        return (rhs >> bit) & 0x01;
    }

    template <class Ty>
    inline void
    BitSet(Ty& rhs, uint8_t bit)
    {
        rhs |= (1UL << bit);
    }

    template <class Ty>
    inline void
    BitClear(Ty& rhs, uint8_t bit)
    {
        rhs &= ~(1UL << bit);
    }

    template <class Ty>
    inline void
    BitToggle(Ty& rhs, uint8_t bit)
    {
        rhs ^= (1UL << bit);
    }

    template <class Ty>
    inline void
    BitWrite(Ty& rhs, uint8_t bit, bool value)
    {
        value ? BitSet(rhs, bit) : BitClear(rhs, bit);
    }

    inline double
    NormalizedAngle(const double angle, const double min, const double max)
    {
        const auto cycle = max - min;

        const auto modAngle = fmod((angle - min), cycle) + min;

        if (modAngle < min)
        {
            return modAngle + cycle;
        }
        else
        {
            return modAngle;
        }
    }

    inline double
    Map(const double value, const double inputMin, const double inputMax, const double outputMin, const double outputMax)
    {
        return (value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
    }

}    // namespace udon
