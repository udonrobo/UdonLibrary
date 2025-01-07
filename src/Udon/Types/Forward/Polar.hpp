//
//    直交座標系 前方定義
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Traits/EnumerableMacro.hpp>
#include <Udon/Serializer/Serializer.hpp>

namespace Udon
{


    struct Vec2;


    /// @brief 極座標系
    struct Polar
    {
        /// @brief 要素の型
        using ValueType = double;

        /// @brief 中心からの距離
        ValueType r;

        /// @brief 角度
        ValueType theta;

        /// @brief デフォルトコンストラクタ
        constexpr Polar() noexcept
            : r()
            , theta()
        {
        }

        /// @brief コンストラクタ
        /// @param r 中心からの距離
        /// @param theta 角度
        constexpr Polar(ValueType r, ValueType theta) noexcept
            : r(r)
            , theta(theta)
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        Polar(const Polar&) = default;

        /// @brief デフォルトコピー代入演算子
        Polar& operator=(const Polar&) = default;

        /// @brief x 座標を取得する
        /// @return
        ValueType x() const noexcept
        {
            return r * cos(theta);
        }

        /// @brief y 座標を取得する
        /// @return
        ValueType y() const noexcept
        {
            return r * sin(theta);
        }

        friend constexpr Polar operator+(const Polar& lhs, const Polar& rhs) noexcept { return { lhs.r + rhs.r, lhs.theta + rhs.theta }; }
        friend constexpr Polar operator-(const Polar& lhs, const Polar& rhs) noexcept { return { lhs.r - rhs.r, lhs.theta - rhs.theta }; }
        friend constexpr Polar operator*(const Polar& lhs, const Polar& rhs) noexcept { return { lhs.r * rhs.r, lhs.theta * rhs.theta }; }
        friend constexpr Polar operator/(const Polar& lhs, const Polar& rhs) noexcept { return { lhs.r / rhs.r, lhs.theta / rhs.theta }; }

        Polar& operator+=(const Polar& rhs) noexcept { return *this = *this + rhs; }
        Polar& operator-=(const Polar& rhs) noexcept { return *this = *this - rhs; }
        Polar& operator*=(const Polar& rhs) noexcept { return *this = *this * rhs; }
        Polar& operator/=(const Polar& rhs) noexcept { return *this = *this / rhs; }

        friend constexpr bool operator==(const Polar& lhs, const Polar& rhs) noexcept { return lhs.r == rhs.r && lhs.theta == rhs.theta; }
        friend constexpr bool operator!=(const Polar& lhs, const Polar& rhs) noexcept { return not(lhs == rhs); }

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        constexpr explicit operator bool() const noexcept { return r or theta; }

        /// @brief ベクトルに変換する
        /// @return
        Vec2 toVector() const noexcept;

        UDON_ENUMERABLE(r, theta);
    };
}    // namespace Udon
