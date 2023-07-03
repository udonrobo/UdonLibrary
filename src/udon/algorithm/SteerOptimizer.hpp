#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <array>

#include <udon/types/Polar.hpp>
#include <udon/math/Math.hpp>

namespace udon
{

    /// @brief 独立ステアリング機構最適化クラス
    /// @remark 1モジュールの最適化を行う(モジュールごとをオブジェクトで管理したいときはこっちがオヌヌメ)
    /// @tparam WheelCount タイヤの数
    class SteerModuleOptimizer
    {

        /// @brief モジュールを無限回転させるためのオフセット
        double offset;

        /// @brief 最適値
        udon::Polar optimized;

    public:
        /// @brief コンストラクタ
        SteerModuleOptimizer()
            : offset()
            , optimized()
        {
        }

        /// @brief 最適化を行う(実測値と比較する)
        /// @param current エンコーダー等から算出した現在の値 (極座標配列 r:[-∞~∞(radians)] theta:[自由])
        /// @param raw     最適化前の値                     (極座標配列 r:[-π~π(radians)] theta:[自由])
        /// @return        最適化後の値                     (極座標配列 r:[-∞~∞(radians)] theta:[±最適化前theta])
        inline auto operator()(
            const udon::Polar& current,
            const udon::Polar& raw) -> udon::Polar
        {
            // タイヤのどれも動いていない -> 前回値
            if (raw.r == 0)
            {
                optimized.r = 0;
                return optimized;
            }

            // atan2 から求めた角度 [-π ~ π] から無限回転に変換
            const auto infinitemized = [](
                                        const udon::Polar& prev,
                                        double&            offsetRef,
                                        const udon::Polar& raw) -> udon::Polar
            {
                // 変化角
                const auto dTheta = raw.theta + offsetRef - prev.theta;

                // 変化量がいきなり半周を超えた -> 計算値が-π~π間を通過 -> 一周分オフセットを加減算
                if (dTheta > udon::Pi)
                {
                    offsetRef -= udon::Pi * 2;
                }
                else if (dTheta < -udon::Pi)
                {
                    offsetRef += udon::Pi * 2;
                }
                return { raw.r, raw.theta + offsetRef };

            }(current, offset, raw);

            // 旋回方向、ホイール回転方向最適化後
            const auto reversibled = [](const Polar& prev,
                                        const Polar& raw) -> udon::Polar
            {
                // 変化角
                const auto dTheta = raw.theta - prev.theta;

                // 90度以上回転するときはホイールを逆回転させ、半周旋回
                if (dTheta > udon::Pi / 2)
                {
                    return { raw.r * -1, raw.theta - udon::Pi };
                }
                else if (dTheta < -udon::Pi / 2)
                {
                    return { raw.r * -1, raw.theta + udon::Pi };
                }
                else
                {
                    return raw;
                }
                
            }(current, infinitemized);

            // 最適化値更新
            optimized = reversibled;

            return optimized;
        }

        /// @brief 最適化を行う(前回の制御値と比較する)
        /// @param raw 最適化前の値 (極座標配列 r:[-π~π(radians)] theta:[自由          ])
        /// @return    最適化後の値 (極座標配列 r:[-∞~∞(radians)] theta:[±最適化前theta])
        auto operator()(
            const udon::Polar& raw) -> udon::Polar
        {
            return (*this)(optimized, raw);
        }
    };

    /// @brief 独立ステアリング機構最適化クラス
    /// @tparam WheelCount タイヤの数
    template <size_t WheelCount = 4>
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

        /// @brief 最適化を行う(実測値と比較する)
        /// @param current エンコーダー等から算出した現在の値 (極座標配列 r:[-∞~∞(radians)] theta:[自由])
        /// @param raw     最適化前の値                     (極座標配列 r:[-π~π(radians)] theta:[自由])
        /// @return        最適化後の値                     (極座標配列 r:[-∞~∞(radians)] theta:[±最適化前theta])
        inline auto operator()(
            const std::array<udon::Polar, WheelCount>& current,
            const std::array<udon::Polar, WheelCount>& raw) -> std::array<udon::Polar, WheelCount>;

        /// @brief 最適化を行う(前回の制御値と比較する)
        /// @param raw 最適化前の値 (極座標配列 r:[-π~π(radians)] theta:[自由　　　     ])
        /// @return    最適化後の値 (極座標配列 r:[-∞~∞(radians)] theta:[±最適化前theta])
        auto operator()(
            const std::array<udon::Polar, WheelCount>& raw) -> std::array<udon::Polar, WheelCount>
        {
            return (*this)(optimized, raw);
        }
    };

    template <size_t WheelCount>
    inline auto SteerOptimizer<WheelCount>::operator()(
        const std::array<udon::Polar, WheelCount>& current,
        const std::array<udon::Polar, WheelCount>& raw) -> std::array<udon::Polar, WheelCount>
    {
        // タイヤのどれも動いていない -> 前回値
        if (std::any_of(raw.begin(), raw.end(), [](const udon::Polar& steer)
                        { return steer.r == 0.; }))
        {
            for (auto&& it : optimized)
            {
                it.r = 0.;
            }
            return optimized;
        }

        for (size_t i = 0; i < optimized.size(); ++i)
        {
            // atan2 から求めた角度 [-π ~ π] から無限回転に変換
            const auto infinitemized = [](
                                           const udon::Polar& prev,
                                           double&            offsetRef,
                                           const udon::Polar& raw) -> udon::Polar
            {
                // 変化角
                const auto dTheta = raw.theta + offsetRef - prev.theta;

                // 変化量がいきなり半周を超えた -> -π~π の間を通過 -> 一周分オフセットを加算
                if (dTheta > udon::Pi)
                {
                    offsetRef -= udon::Pi * 2;
                }
                else if (dTheta < -udon::Pi)
                {
                    offsetRef += udon::Pi * 2;
                }
                return { raw.r, raw.theta + offsetRef };
            }(current.at(i), offset.at(i), raw.at(i));

            // 旋回方向、ホイール回転方向最適化後
            const auto reversibled = [](const Polar& prev,
                                        const Polar& raw) -> udon::Polar
            {
                // 変化角
                const auto dTheta = raw.theta - prev.theta;

                // 90度以上回転するときはホイールを逆回転させ、半周旋回
                if (dTheta > udon::Pi / 2)
                {
                    return { raw.r * -1, raw.theta - udon::Pi };
                }
                else if (dTheta < -udon::Pi / 2)
                {
                    return { raw.r * -1, raw.theta + udon::Pi };
                }
                else
                {
                    return raw;
                }
            }(current.at(i), infinitemized);

            // 最適化値更新
            optimized.at(i) = reversibled;
        }

        return optimized;
    }


}    // namespace udon
