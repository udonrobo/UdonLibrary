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
//    数学関数
//
//-------------------------------------------------------------------


#pragma once

#include <math.h>

#include <Udon/Math/Constants.hpp>

namespace Udon
{

    /// @brief 2つの値のうち小さい方を返す
    template <typename A, typename B>
    inline constexpr auto
    Min(const A& lhs, const B& rhs) -> decltype(lhs + rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }

    /// @brief 2つの値のうち大きい方を返す
    template <typename A, typename B>
    inline constexpr auto
    Max(const A& lhs, const B& rhs) -> decltype(lhs + rhs)
    {
        return lhs > rhs ? lhs : rhs;
    }

    /// @brief 値を指定された範囲内に収める
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

    /// @brief 絶対値を返す
    /// @param rhs 絶対値を求めたい値
    /// @return    絶対値
    template <typename T>
    inline constexpr T
    Abs(const T& rhs)
    {
        return rhs < 0 ? -rhs : rhs;
    }

    /// @brief 二乗を求める
    /// @param rhs 二乗を求めたい値
    /// @return
    template <typename T>
    inline constexpr T
    Sq(const T& x)
    {
        return x * x;
    }

    /// @brief 小数点切り捨て
    inline constexpr int
    Floor(double x)
    {
        return x - (int)x < 0 ? (int)x - 1 : (int)x;
    }

    /// @brief 四捨五入
    inline constexpr int
    Round(double x)
    {
        return x - (int)x >= 0.5 ? (int)x + 1 : (int)x;
    }

    /// @brief 小数点切り上げ
    inline constexpr int 
    Ceil(double x)
    {
        return x - (int)x > 0 ? (int)x + 1 : (int)x;
    }

    /// @brief 数値をある範囲から別の範囲に再マッピングする
    inline double
    Map(const double value, const double inputMin, const double inputMax, const double outputMin, const double outputMax)
    {
        return (value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
    }

    /// @brief 値を正規化する
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

}    // namespace Udon
