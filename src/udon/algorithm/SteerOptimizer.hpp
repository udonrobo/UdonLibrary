#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <array>

#include <udon/types/Polar.hpp>

namespace udon
{

    /// @brief 独立ステアリング機構最適化クラス
    /// @tparam WheelCount タイヤの数
    template <size_t WheelCount>
    class SteerOptimizer
    {

        /// @brief モジュールを無限回転させるためのオフセット
        std::array<double, WheelCount> offset;

        /// @brief 最適値
        std::array<udon::Polar, WheelCount> optimized;

    public:
        /// @brief コンストラクタ
        SteerOptimizer()
            : offset()
            , optimized()
        {
        }

        /// @brief 最適化した値を取得、更新
        /// @param raw 最適化前の値(極座標配列 r:[-π~π(radians)] theta:[自由])
        /// @return 最適化後の値(極座標配列 r:[-∞~∞(radians)] theta:[±最適化前theta])
        auto operator()(const std::array<udon::Polar, WheelCount>& raw)
            -> std::array<udon::Polar, WheelCount>
        {
            // タイヤのどれも動いていない -> 前回値
            if (not std::any_of(raw.begin(), raw.end(), [](const udon::Polar& steer)
                                { return steer.r > 0.; }))
            {
                for (auto&& it : optimized)
                {
                    it.r = 0.;
                }
                return optimized;
            }

            // タイヤのどれか一つでも動いている時、最適化
            for (size_t i = 0; i < optimized.size(); ++i)
            {
                // atan2 から求めた角度 [-π ~ π] から無限回転に変換
                const auto infinitemized = [](const udon::Polar& prev,
                                              double&            offsetRef,
                                              const udon::Polar& raw) -> udon::Polar
                {
                    const auto dAngle = raw.theta + offsetRef - prev.theta;    // 変化角

                    // 変化量がいきなり半周を超えた -> -π~π の間を通過 -> 一周分オフセットを加算
                    if (dAngle > udon::Pi)
                    {
                        offsetRef -= udon::Pi * 2;
                    }
                    else if (dAngle < -udon::Pi)
                    {
                        offsetRef += udon::Pi * 2;
                    }
                    return { raw.r, raw.theta + offsetRef };
                }(optimized.at(i), offset.at(i), raw.at(i));

                // 旋回方向、ホイール回転方向最適化
                const auto reversibled = [](const Polar& prev,
                                            const Polar& raw) -> udon::Polar
                {
                    const auto dAngle = raw.theta - prev.theta;    // 変化角

                    // 90度以上回転するときはホイールを逆回転させ、半周旋回
                    if (dAngle > udon::Pi / 2)
                    {
                        return { raw.r * -1, raw.theta - udon::Pi };
                    }
                    else if (dAngle < -udon::Pi / 2)
                    {
                        return { raw.r * -1, raw.theta + udon::Pi };
                    }
                    else
                    {
                        return raw;
                    }
                }(optimized.at(i), infinitemized);

                // 最適化値更新
                optimized.at(i) = reversibled;
            }

            return optimized;

        }    // operator()
    };
}    // namespace udon
