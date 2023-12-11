//
//    独立ステアリング機構最適化クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 Udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <Udon/Types/Polar.hpp>
#include <array>

#include "Math.hpp"

namespace Udon
{

    /// @brief 独立ステアリング機構の各モジュール最適化クラス
    /// @remark 1モジュールの最適化を行う(モジュールごとをオブジェクトで管理する場合はこっちがオヌヌメ)
    class SteerModuleOptimizer
    {

        /// @brief モジュールを無限回転させるためのオフセット
        double offset;

        /// @brief 最適値
        Udon::Polar optimized;

    public:
        /// @brief コンストラクタ
        SteerModuleOptimizer()
            : offset()
            , optimized()
        {
        }

        /// @brief 最適化値を消去する
        /// @remark 旋回角のゼロ点をリセットするとき等に使う
        void clear()
        {
            offset    = 0;
            optimized = {};
        }

        /// @brief 最適化を行う(実測値と比較する)
        /// @param current エンコーダー等から算出した現在の値 (極座標 r:[-∞~∞(radians)] theta:[自由])
        /// @param target  最適化前の値                      (極座標 r:[-π~π(radians)] theta:[自由])
        /// @return        最適化後の値                      (極座標 r:[-∞~∞(radians)] theta:[±最適化前theta])
        Udon::Polar operator()(const Udon::Polar& current, const Udon::Polar& target)
        {
            // タイヤのどれも動いていない -> 前回値
            if (target.r == 0)
            {
                optimized.r = 0;
                return optimized;
            }

            // atan2 等から求めた角度 [-π ~ π] から無限回転に変換
            const auto infinitemized = WrapAngle(current, /*ref*/ offset, target);

            // 旋回方向、ホイール回転方向最適化後
            const auto reversibled = OptimizeSteeringAndWheelRotation(current, infinitemized);

            // 最適化値更新
            optimized = reversibled;

            return optimized;
        }

        /// @brief 最適化を行う(前回の制御値と比較する)
        /// @param target 最適化前の値 (極座標 r:[-π~π(radians)] theta:[自由          ])
        /// @return       最適化後の値 (極座標 r:[-∞~∞(radians)] theta:[±最適化前theta])
        Udon::Polar operator()(const Udon::Polar& target)
        {
            return (*this)(optimized, target);
        }

    private:
        /// @brief 無限回転に変換(副作用あり*)
        /// @param current 現在の値
        /// @param offsetRef 無限回転にするためのオフセット(*)
        /// @param target 目標値
        /// @return 最適化後の値
        static Udon::Polar WrapAngle(const Udon::Polar& current, double& offsetRef, const Udon::Polar& target)
        {
            // 変化角
            const auto dTheta = target.theta + offsetRef - current.theta;

            // 変化量がいきなり半周を超えた -> 計算値が-π~π間を通過 -> 一周分オフセットを加減算
            if (dTheta > Udon::Pi)
            {
                offsetRef -= Udon::Pi * 2;
            }
            else if (dTheta < -Udon::Pi)
            {
                offsetRef += Udon::Pi * 2;
            }
            return { target.r, target.theta + offsetRef };
        }

        /// @brief 旋回方向、ホイール回転方向最適化
        /// @param current 現在の値
        /// @param target 目標値
        /// @return 最適化後の値
        static Udon::Polar OptimizeSteeringAndWheelRotation(const Polar& current, const Polar& target)
        {
            // 変化角
            const auto dTheta = target.theta - current.theta;

            // 90度以上回転するときはホイールを逆回転させ、半周旋回
            if (dTheta > Udon::Pi / 2)
            {
                return { target.r * -1, target.theta - Udon::Pi };
            }
            else if (dTheta < -Udon::Pi / 2)
            {
                return { target.r * -1, target.theta + Udon::Pi };
            }
            else
            {
                return target;
            }
        }
    };

    /// @brief 独立ステアリング機構最適化クラス
    /// @tparam WheelCount タイヤの数
    template <size_t WheelCount = 4>
    class SteerOptimizer
    {

        /// @brief モジュール
        std::array<SteerModuleOptimizer, WheelCount> modules;

    public:
        /// @brief コンストラクタ
        SteerOptimizer() = default;

        /// @brief 最適化値を消去する
        /// @remark 旋回角のゼロ点をリセットするとき等に使う
        void clear()
        {
            for (auto&& module : modules)
            {
                module.clear();
            }
        }

        /// @brief 最適化を行う(実測値と比較する)
        /// @param current エンコーダー等から算出した現在の値 (極座標配列 r:[-∞~∞(radians)] theta:[自由])
        /// @param target  最適化前の値                      (極座標配列 r:[-π~π(radians)] theta:[自由])
        /// @return        最適化後の値                      (極座標配列 r:[-∞~∞(radians)] theta:[±最適化前theta])
        inline auto operator()(
            const std::array<Udon::Polar, WheelCount>& current,
            const std::array<Udon::Polar, WheelCount>& target) -> std::array<Udon::Polar, WheelCount>
        {
            std::array<Udon::Polar, WheelCount> optimized;
            for (size_t i = 0; i < WheelCount; ++i)
            {
                optimized.at(i) = modules.at(i)(current.at(i), target.at(i));
            }
            return optimized;
        }

        /// @brief 最適化を行う(前回の制御値と比較する)
        /// @param target 最適化前の値 (極座標配列 r:[-π~π(radians)] theta:[自由])
        /// @return       最適化後の値 (極座標配列 r:[-∞~∞(radians)] theta:[±最適化前theta])
        auto operator()(const std::array<Udon::Polar, WheelCount>& target) -> std::array<Udon::Polar, WheelCount>
        {
            std::array<Udon::Polar, WheelCount> optimized;
            for (size_t i = 0; i < WheelCount; ++i)
            {
                optimized.at(i) = modules.at(i)(target.at(i));
            }
            return optimized;
        }
    };

}    // namespace Udon
