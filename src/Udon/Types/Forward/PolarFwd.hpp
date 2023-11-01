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
//    直交座標系 前方定義
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Traits/ParsableMacro.hpp>
#include <Udon/Com/Serialization.hpp>

namespace Udon
{
    struct Vector2D;

    struct Polar
    {
        using ValueType = double;

        ValueType r;
        ValueType theta;

        constexpr Polar() noexcept
            : r()
            , theta()
        {
        }

        constexpr Polar(ValueType r, ValueType theta) noexcept
            : r(r)
            , theta(theta)
        {
        }

        Polar(const Polar&) = default;

        Polar& operator=(const Polar&) = default;

        ValueType x() const noexcept
        {
            return r * cos(theta);
        }

        ValueType y() const noexcept
        {
            return r * sin(theta);
        }

        constexpr Polar operator+(const Polar& rhs) const noexcept { return { r + rhs.r, theta + rhs.theta }; }
        constexpr Polar operator-(const Polar& rhs) const noexcept { return { r - rhs.r, theta - rhs.theta }; }
        constexpr Polar operator*(const Polar& rhs) const noexcept { return { r * rhs.r, theta * rhs.theta }; }
        constexpr Polar operator/(const Polar& rhs) const noexcept { return { r / rhs.r, theta / rhs.theta }; }

        Polar& operator+=(const Polar& rhs) noexcept { return *this = *this + rhs; }
        Polar& operator-=(const Polar& rhs) noexcept { return *this = *this - rhs; }
        Polar& operator*=(const Polar& rhs) noexcept { return *this = *this * rhs; }
        Polar& operator/=(const Polar& rhs) noexcept { return *this = *this / rhs; }

        constexpr bool operator==(const Polar& rhs) const noexcept { return r == rhs.r && theta == rhs.theta; }
        constexpr bool operator!=(const Polar& rhs) const noexcept { return !(*this == rhs); }

        constexpr explicit operator bool() const noexcept { return r || theta; }

        Vector2D toVector() const noexcept;

        UDON_PARSABLE(r, theta);
    };
}    // namespace Udon
