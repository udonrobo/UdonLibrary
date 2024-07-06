#pragma once

#include <Udon/Algorithm/Math.hpp>
#include <Udon/Types/Position.hpp>

namespace Udon
{

    namespace Impl
    {

        /// @brief 出力値のうち、最も大きい値が指定値を超える場合、指定値を超えないように全ての出力値を同じ比率で圧縮する
        template <size_t N>
        std::array<double, N> CompressPowers(const std::array<double, N>& powers, uint8_t powerLimit)
        {
            // 最も大きい出力値を求める
            int maxPower = 0;
            for (auto&& power : powers)
            {
                maxPower = std::max(maxPower, std::abs(power));    // 負の値も考慮する
            }

            // 最大出力値が指定値を超える場合、全ての出力値を同じ比率で圧縮する
            if (maxPower > powerLimit)
            {
                const auto ratio = powerLimit / maxPower;

                for (auto&& power : powers)
                {
                    power *= ratio;
                }
            }

            return powers;
        }

    }    // namespace Impl

    /// @brief スティックの入力値からオムニホイールの出力値を算出する
    /// @note 4輪オムニの場合、単純な加算で出力値を算出できる
    /// @param stick
    /// @param powerLimit
    /// @param turnPowerLimit
    /// @return
    template <size_t N>
    std::array<double, N> ToOmni(const Udon::Stick& stick, uint8_t powerLimit = 255, uint8_t turnPowerLimit = 255)
    {
    }


    /// @brief 4輪オムニの場の部分特殊化
    /// @note 4輪オムニの場合、単純な加減算で出力値を算出できる
    std::array<double, 4> ToOmni<4>(const Udon::Stick& stick, uint8_t powerLimit = 255, uint8_t turnPowerLimit = 255)
    {
        const auto turn = Udon::Map(stick.turn, -255, 255, -turnPowerLimit, turnPowerLimit);

        const auto vector = stick.vector;

        const std::array<double, 4> powers{ {
            +vector.x + -vector.y + turn,    // 右上
            -vector.x + -vector.y + turn,    // 右下
            -vector.x + +vector.y + turn,    // 左下
            +vector.x + +vector.y + turn,    // 左上
        } };

        return Impl::CompressPowers(powers, powerLimit);
    }


    struct OmniModule
    {
        double angle;    // タイヤ取り付け角度 [rad]
        double r = 1;    // 中央からの距離 [比率]
    };


    /// @brief スティックの入力値からオムニホイールの出力値を算出する
    /// @note 取り付け角度を考慮して出力値を算出する
    template <size_t N>
    std::array<double, N> ToOmni(const std::array<OmniModule, N>& modules, const Udon::Stick& stick, uint8_t powerLimit = 255, uint8_t turnPowerLimit = 255)
    {

        const auto turn = Udon::Map(stick.turn, -255, 255, -turnPowerLimit, turnPowerLimit);

        std::array<double, N> powers;

        double maxR = 0;
        for (size_t i = 0; i < N; ++i)
        {
            maxR = std::max(maxR, modules.at(i).r);
        }

        for (size_t i = 0; i < N; ++i)
        {
            const auto vector = stick.vector.rotated(modules.at(i).angle);

            powers.at(i) = vector.x + vector.y + turn * modules.at(i).r / maxR;
        }

        return Impl::CompressPowers(powers, powerLimit);
    }

}    // namespace Udon