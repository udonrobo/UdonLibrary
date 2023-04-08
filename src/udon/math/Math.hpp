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

    template <class L, class R>
    inline constexpr auto
    Min(const L& lhs, const R& rhs) -> decltype(lhs + rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }

    template <class L, class R>
    inline constexpr auto
    Max(const L& lhs, const R& rhs) -> decltype(lhs + rhs)
    {
        return lhs > rhs ? lhs : rhs;
    }

    template <class Ty>
    inline constexpr Ty
    Abs(const Ty& rhs)
    {
        return rhs > 0 ? rhs : -rhs;
    }

    template <class A, class B>
    inline constexpr A
    Constrain(const A& amt, const B& low, const B& high)
    {
        return udon::Min(udon::Max(amt, low), high);
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
        value ? bitSet(rhs, bit) : bitClear(rhs, bit);
    }

}    // namespace udon