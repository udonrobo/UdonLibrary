#pragma once

#include <array>
#include <udon/stl/EnableSTL.hpp>

#include <udon/math/Math.hpp>
#include <udon/types/Polar.hpp>
#include <udon/types/Vector2D.hpp>

namespace udon
{

    /// @brief ロボットの位置
    /// @tparam T 要素の型
    template <typename T>
    struct Position
    {

        /// @brief 要素の型
        using value_type = T;

        /// @brief 座標
        udon::Vector2D<value_type> vector;

        /// @brief 旋回角 [rad]
        value_type turn;

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
        constexpr Position(const udon::Vector2D<value_type>& vector, value_type turn) noexcept
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
        constexpr Position operator+(value_type rhs) const noexcept { return { vector + rhs, turn + rhs }; }
        constexpr Position operator-(value_type rhs) const noexcept { return { vector - rhs, turn - rhs }; }
        constexpr Position operator*(value_type rhs) const noexcept { return { vector * rhs, turn * rhs }; }
        constexpr Position operator/(value_type rhs) const noexcept { return { vector / rhs, turn / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        Position operator+=(const Position& rhs) noexcept { return *this = *this + rhs; }
        Position operator-=(const Position& rhs) noexcept { return *this = *this - rhs; }
        Position operator*=(const Position& rhs) noexcept { return *this = *this * rhs; }
        Position operator/=(const Position& rhs) noexcept { return *this = *this / rhs; }
        Position operator+=(value_type rhs) noexcept { return *this = *this + rhs; }
        Position operator-=(value_type rhs) noexcept { return *this = *this - rhs; }
        Position operator*=(value_type rhs) noexcept { return *this = *this * rhs; }
        Position operator/=(value_type rhs) noexcept { return *this = *this / rhs; }

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        constexpr operator bool() const noexcept
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

        // template <size_t WheelCount>
        // std::array<int16_t, WheelCount> toOmni() const noexcept
        //{
        //     std::array<int16_t, WheelCount> omni;
        //     // todo
        //     return omni;
        // }

        /// @brief 独立ステアリング機構のタイヤ出力値、旋回角を取得する
        /// @param powerLimit ホイール出力値の最大値
        /// @param turnPowerLimit 旋回の最大値
        /// @return 各モジュールの値
        ///   ↑      ↑
        ///  |３|    |０|
        ///
        ///   ↑      ↑
        ///  |２|    |１|
        template <size_t WheelCount = 4>
        std::array<udon::Polar, WheelCount> toSteer(uint8_t powerLimit = 255, uint8_t turnPowerLimit = 255) const noexcept
        {
            // 旋回移動ベクトル
            const udon::Vec2 turnVec = { 0, udon::Map(this->turn, -255, 255, -turnPowerLimit, turnPowerLimit) };

            // 旋回移動ベクトル、進行移動ベクトルから角車輪の進行方向ベクトルを算出
            std::array<udon::Vec2, WheelCount> vectors;

            for (size_t i = 0; i < WheelCount; ++i)
            {
                vectors.at(i) = this->vector + turnVec.rotated(udon::Pi * (i * 2 + 3) / WheelCount);
            }

            // 方向ベクトルから極座標系(theta:旋回角, r:タイヤ出力)に変換
            std::array<udon::Polar, WheelCount> modules;

            for (size_t i = 0; i < WheelCount; ++i)
            {
                modules.at(i) = vectors.at(i).toPolar();
            }

            // 上で算出した出力値は {limit} を超える場合があります。
            // よって超えた場合、最大出力のモジュールの出力を {limit} として他のモジュールの出力を圧縮します。
            // 出力値はベクトルの長さから求めるため負にならない {max} は正の値であることが保証
            auto&& max = *std::max_element(modules.begin(), modules.end(), [](const udon::Polar& lhs, const udon::Polar& rhs)
                                           { return lhs.r < rhs.r; });

            if (max.r > powerLimit)
            {
                const auto ratio = powerLimit / max.r;
                std::for_each(modules.begin(), modules.end(), [ratio](udon::Polar& module)
                              { module.r *= ratio; });
            }

            return modules;
        }

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            vector.show();
            Serial.print(F("turn: ")), Serial.print(true), Serial.print('\t');
        }
#endif

        /// @brief シリアライズ後のバイト数を求める
        /// @return
        constexpr size_t capacity() const
        {
            return udon::Capacity(vector, turn);
        }

        /// @brief
        /// @tparam T
        /// @param acc
        template <typename Acc>
        void accessor(Acc& acc)
        {
            acc(vector, turn);
        }
    };

    using Pos = Position<double>;

}    // namespace udon
