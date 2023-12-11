//
//    ロボットの位置 (座標、旋回角) を表す型
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>
#include <array>

#include "Polar.hpp"
#include "Vector2D.hpp"

#include <Udon/Algorithm/Math.hpp>
#include <Udon/Traits/ParsableMacro.hpp>
#include <Udon/Com/Serialization.hpp>

namespace Udon
{

    /// @brief ロボットの位置
    struct Position
    {

        /// @brief 要素の型
        using ValueType = double;

        /// @brief 座標
        Udon::Vector2D vector;

        /// @brief 旋回角 [rad]
        ValueType turn;

        /// @brief デフォルトコンストラクタ
        constexpr Position() noexcept
            : vector()
            , turn()
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        constexpr Position(const Position& rhs) noexcept
            : vector(rhs.vector)
            , turn(rhs.turn)
        {
        }

        /// @brief コンストラクタ
        constexpr Position(const Udon::Vector2D& vector, ValueType turn) noexcept
            : vector(vector)
            , turn(turn)
        {
        }

        /// @brief デフォルトコピー代入演算子
        Position& operator=(const Position&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Position operator+(const Position& rhs) const noexcept { return { vector + rhs.vector, turn + rhs.turn }; }
        constexpr Position operator-(const Position& rhs) const noexcept { return { vector - rhs.vector, turn - rhs.turn }; }
        constexpr Position operator*(const Position& rhs) const noexcept { return { vector * rhs.vector, turn * rhs.turn }; }
        constexpr Position operator/(const Position& rhs) const noexcept { return { vector / rhs.vector, turn / rhs.turn }; }
        constexpr Position operator+(ValueType rhs) const noexcept { return { vector + rhs, turn + rhs }; }
        constexpr Position operator-(ValueType rhs) const noexcept { return { vector - rhs, turn - rhs }; }
        constexpr Position operator*(ValueType rhs) const noexcept { return { vector * rhs, turn * rhs }; }
        constexpr Position operator/(ValueType rhs) const noexcept { return { vector / rhs, turn / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        Position operator+=(const Position& rhs) noexcept { return *this = *this + rhs; }
        Position operator-=(const Position& rhs) noexcept { return *this = *this - rhs; }
        Position operator*=(const Position& rhs) noexcept { return *this = *this * rhs; }
        Position operator/=(const Position& rhs) noexcept { return *this = *this / rhs; }
        Position operator+=(ValueType rhs) noexcept { return *this = *this + rhs; }
        Position operator-=(ValueType rhs) noexcept { return *this = *this - rhs; }
        Position operator*=(ValueType rhs) noexcept { return *this = *this * rhs; }
        Position operator/=(ValueType rhs) noexcept { return *this = *this / rhs; }

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Position& rhs) const noexcept { return vector == rhs.vector && turn == rhs.turn; }

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator!=(const Position& rhs) const noexcept { return !(*this == rhs); }

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        constexpr explicit operator bool() const noexcept
        {
            return vector || turn;
        }

        /// @brief 要素がすべて0であるかを変えす
        constexpr bool isZero() const noexcept
        {
            return !Position::operator bool();
        }

        /// @brief 値クリア
        void clear() noexcept
        {
            *this = {};
        }

        Position maped(double inMin, double inMax, double outMin, double outMax) const noexcept
        {
            return {
                vector.maped(inMin, inMax, outMin, outMax),
                Map(turn, inMin, inMax, outMin, outMax),
            };
        }

        Position updateVector(const Udon::Vector2D& v) const noexcept
        {
            return { v, turn };
        }

        Position updateTurn(double t) const noexcept
        {
            return { vector, t };
        }

        // template <size_t WheelCount>
        // std::array<double, WheelCount> toOmni() const
        //{
        //     static_assert(WheelCount >= 3, "WheelCount must be greater than or equal to 3.");
        //     static_assert(WheelCount == 4, "Not supported over 4 wheels now.");
        // }

        // template <>
        // std::array<double, 4> toOmni<4>() const
        //{
        //     uint8_t    powerLimit     = 255;
        //             uint8_t turnPowerLimit = 255;
        //     const auto mapedTurn = Udon::Map(turn, -255, 255, -turnPowerLimit, turnPowerLimit);

        //    std::array<double, 4> powers{ {
        //        +vector.x + -vector.y + mapedTurn,
        //        -vector.x + -vector.y + mapedTurn,
        //        -vector.x + +vector.y + mapedTurn,
        //        +vector.x + +vector.y + mapedTurn,
        //    } };

        //    // 上で算出した出力値は {powerLimit} を超える場合があります。
        //    // 超えた場合、最大出力のモジュールの出力を {powerLimit} として他のモジュールの出力を圧縮します。
        //    auto&& max = abs(*std::max_element(powers.begin(), powers.end(), [](double lhs, double rhs)
        //                                       { return abs(lhs.r) < abs(rhs.r); }));

        //    if (max > powerLimit)
        //    {
        //        const auto ratio = powerLimit / max;

        //        std::transform(powers.begin(), powers.end(), powers.begin(),
        //                       [ratio](double power)
        //                       { return power * ratio; });
        //    }

        //    // todo
        //    return powers;
        //}

        /// @brief 独立ステアリング機構のタイヤ出力値、旋回角を取得する
        /// @tparam WheelCount タイヤの数
        /// @param powerLimit ホイール出力値の最大値
        /// @param turnPowerLimit 旋回の最大値
        /// @return ステアの値 (極座標[r:-255~255,theta:[-π~π])の配列)
        /// @details index順 (頂点から時計回りに 0,1,2...)
        ///   ↑      ↑
        ///  |３|    |０|
        ///
        ///   ↑      ↑
        ///  |２|    |１|
        template <size_t WheelCount = 4>
        std::array<Udon::Polar, WheelCount> toSteer(uint8_t powerLimit     = 255,
                                                    uint8_t turnPowerLimit = 255) const
        {
            static_assert(WheelCount >= 3, "WheelCount must be greater than or equal to 3.");
            static_assert(WheelCount == 4, "Not supported over 4 wheels now.");    // todo: 5輪以上対応

            // 旋回移動ベクトル
            const Udon::Vec2 turnVec = { 0, Udon::Map(this->turn, -255, 255, -turnPowerLimit, turnPowerLimit) };

            // 旋回移動ベクトル、進行移動ベクトルから角車輪の進行方向ベクトルを算出
            std::array<Udon::Vec2, WheelCount> vectors;

            for (size_t i = 0; i < WheelCount; ++i)
            {
                vectors.at(i) = vector + turnVec.rotated(Udon::Pi * (i * 2 + 3) / WheelCount);
            }

            // 方向ベクトルから極座標系(theta:旋回角, r:タイヤ出力)に変換
            std::array<Udon::Polar, WheelCount> modules;

            for (size_t i = 0; i < WheelCount; ++i)
            {
                modules.at(i) = vectors.at(i).toPolar();
            }

            // 上で算出した出力値は {limit} を超える場合があります。
            // 超えた場合、最大出力のモジュールの出力を {limit} として他のモジュールの出力を圧縮します。
            // 出力値はベクトルの長さから求めるため負にならない {max} は正の値であることが保証
            auto&& max = *std::max_element(modules.begin(), modules.end(), [](const Udon::Polar& lhs, const Udon::Polar& rhs)
                                           { return lhs.r < rhs.r; });

            if (max.r > powerLimit)
            {
                const auto ratio = powerLimit / max.r;
                for (auto&& module : modules)
                {
                    module.r *= ratio;
                }
            }

            return modules;
        }

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            vector.show();
            Serial.print(F("turn: ")), Serial.print(turn), Serial.print('\t');
        }
#endif

        UDON_PARSABLE(vector, turn);
    };

    using Pos   = Position;
    using Stick = Pos;

    using Stick = Pos;

}    // namespace Udon