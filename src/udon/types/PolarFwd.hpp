//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  直交座標系前方定義
//
//-----------------------------------------------


#pragma once

#include <udon/utility/Parsable.hpp>
#include <udon/com/serialization/Serializer.hpp>

namespace udon
{
    template <typename T>
    struct Vector2D;

    template <typename T>
    struct Polar_impl
    {
        using value_type = T;

        value_type r;
        value_type theta;

        constexpr Polar_impl() noexcept
            : r()
            , theta()
        {
        }

        constexpr Polar_impl(value_type r, value_type theta) noexcept
            : r(r)
            , theta(theta)
        {
        }

        Polar_impl(const Polar_impl&) = default;

        Polar_impl& operator=(const Polar_impl&) = default;

        value_type x() const noexcept
        {
            return r * cos(theta);
        }

        value_type y() const noexcept
        {
            return r * sin(theta);
        }

        constexpr Polar_impl operator+(const Polar_impl& rhs) const noexcept { return { r + rhs.r, theta + rhs.theta }; }
        constexpr Polar_impl operator-(const Polar_impl& rhs) const noexcept { return { r - rhs.r, theta - rhs.theta }; }
        constexpr Polar_impl operator*(const Polar_impl& rhs) const noexcept { return { r * rhs.r, theta * rhs.theta }; }
        constexpr Polar_impl operator/(const Polar_impl& rhs) const noexcept { return { r / rhs.r, theta / rhs.theta }; }

        Polar_impl& operator+=(const Polar_impl& rhs) noexcept { return *this = *this + rhs; }
        Polar_impl& operator-=(const Polar_impl& rhs) noexcept { return *this = *this - rhs; }
        Polar_impl& operator*=(const Polar_impl& rhs) noexcept { return *this = *this * rhs; }
        Polar_impl& operator/=(const Polar_impl& rhs) noexcept { return *this = *this / rhs; }

        constexpr bool operator==(const Polar_impl& rhs) const noexcept { return r == rhs.r && theta == rhs.theta; }
        constexpr bool operator!=(const Polar_impl& rhs) const noexcept { return !(*this == rhs); }

        constexpr explicit operator bool() const noexcept { return r || theta; }

        Vector2D<value_type> toVector() const noexcept;

        UDON_PARSABLE(r, theta);
    };
}    // namespace udon
