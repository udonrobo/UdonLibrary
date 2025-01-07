//
//    数学定数、関数
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <math.h>
#include <Udon/Types/Range.hpp>

#ifdef abs
#   undef abs

template <typename T>
inline constexpr T abs(const T& rhs)
{
    return rhs < 0 ? -rhs : rhs;
}

#endif

namespace Udon
{

    /// @brief π
    constexpr double Pi       = 3.1415926535897932384626433832795;      // std::numbers::pi_v<double>       (C++20)

    /// @brief π/2
    constexpr double HalfPi   = 1.5707963267948966192313216916398;      // std::numbers::pi_v<double> / 2   (C++20)

    /// @brief π*2
    constexpr double TwoPi    = 6.283185307179586476925286766559;       // std::numbers::pi_v<double> * 2   (C++20)

    /// @brief 度数法から弧度法に変換する係数
    constexpr double DegToRad = 0.017453292519943295769236907684886;    // std::numbers::pi_v<double> / 180 (C++20)

    /// @brief 弧度法から度数法に変換する係数
    constexpr double RadToDeg = 57.295779513082320876798154814105;      // 180 / std::numbers::pi_v<double> (C++20)

};    // namespace Udon

namespace Udon
{

    /// @brief 2つの値のうち小さい方を返す (std::min)
    template <typename A, typename B>
    inline constexpr auto
    Min(const A& lhs, const B& rhs) -> decltype(lhs, rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }

    /// @brief 2つの値のうち大きい方を返す (std::max)
    template <typename A, typename B>
    inline constexpr auto
    Max(const A& lhs, const B& rhs) -> decltype(lhs, rhs)
    {
        return lhs > rhs ? lhs : rhs;
    }

    /// @brief 値を指定された範囲内に収める (std::clamp)
    /// @param amt  範囲内に収めたい値
    /// @param low  最小値
    /// @param high 最大値
    /// @return     範囲内に収めた値
    template <typename A, typename B, typename C>
    inline constexpr A
    Constrain(const A& amt, const B& low, const C& high)
    {
        return Min(Max(amt, low), high);
    }

    /// @brief 値を指定された範囲内に収める (std::clamp)
    /// @param amt  範囲内に収めたい値
    /// @param range 範囲
    /// @return     範囲内に収めた値
    template <typename A, typename B, typename C = B>
    inline constexpr A
    Constrain(const A& amt, const Udon::Range<B, C>& range)
    {
        return Min(Max(amt, range.min), range.max);
    }

    /// @brief 度数法の角度を弧度法に変換する
    /// @param rhs 度数法の角度
    /// @return    弧度法の角度
    template <typename T>
    inline constexpr T
    ToRadians(const T& rhs)
    {
        return rhs * DegToRad;
    }

    /// @brief 弧度法の角度を度数法に変換する
    /// @param rhs 弧度法の角度
    /// @return    度数法の角度
    template <typename T>
    inline constexpr T
    ToDegrees(const T& rhs)
    {
        return rhs * RadToDeg;
    }

    /// @brief 絶対値を返す (std::abs)
    /// @param rhs 絶対値を求めたい値
    /// @return    絶対値
    template <typename T>
    inline constexpr T
    Abs(const T& rhs)
    {
        return rhs < 0 ? -rhs : rhs;
    }

    /// @brief 二乗を求める (std::pow(x, 2))
    /// @param rhs 二乗を求めたい値
    /// @return
    template <typename T>
    inline constexpr T
    Sq(const T& x)
    {
        return x * x;
    }

    /// @brief 小数点切り捨て (std::floor)
    inline constexpr int
    Floor(double x)
    {
        return x - (int)x < 0 ? (int)x - 1 : (int)x;
    }

    /// @brief 四捨五入 (std::round)
    inline constexpr int
    Round(double x)
    {
        return x - (int)x >= 0.5 ? (int)x + 1 : (int)x;
    }

    /// @brief 小数点切り上げ (std::ceil)
    inline constexpr int
    Ceil(double x)
    {
        return x - (int)x > 0 ? (int)x + 1 : (int)x;
    }

    /// @brief 数値をある範囲から別の範囲に再マッピングする
    inline constexpr double
    Map(const double value, const double inputMin, const double inputMax, const double outputMin, const double outputMax)
    {
        return (value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
    }

    // inline bool
    // FloatEq(const double l, const double r)
    // {
    //     return 
    // }

    /// @brief 値を正規化する
    /// @note -∞~+∞ の範囲を min~max の範囲に変換する (剰余算に最小値が設定できるようになったイメージ)
    /// @param value 正規化したい値
    /// @param min   最小値
    /// @param max   最大値
    /// @return      正規化された値
    inline double
    Normalized(double value, double min, double max)
    {
        const auto cycle = max - min;

        const auto modValue = fmod((value - min), cycle) + min;

        if (modValue < min)
        {
            return modValue + cycle;
        }
        else
        {
            return modValue;
        }
    }

    /// @brief 三平方の定理を用いて、2辺の長さから斜辺の長さを求める
    /// @param x X軸方向の辺の長さ
    /// @param y Y軸方向の辺の長さ
    /// @return 斜辺の長さ
    inline double
    Hypotenuse(double x, double y)
    {
        return sqrt(x * x + y * y);
    }

}    // namespace Udon
