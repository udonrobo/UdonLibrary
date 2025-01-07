//
//    オイラー角 前方定義
//
//    Copyright (c) 2022 udonrobo
//


#pragma once

#include <Udon/Algorithm/Math.hpp>    // Udon::Normalized
#include <Udon/Traits/EnumerableMacro.hpp>
#include <Udon/Serializer/Serializer.hpp>

namespace Udon
{

    struct Quaternion;


    /// @brief オイラー角の回転方向
    struct EulerDirection
    {
        bool roll;
        bool pitch;
        bool yaw;
    };


    /// @brief オイラー角
    struct Euler
    {

        /// @brief 要素の型
        using ValueType = double;

        /// @brief ロール角
        ValueType roll;

        /// @brief ピッチ角
        ValueType pitch;

        /// @brief ヨー角
        ValueType yaw;

        /// @brief デフォルトコンストラクタ
        constexpr Euler() noexcept
            : roll()
            , pitch()
            , yaw()
        {
        }

        /// @brief コンストラクタ
        /// @param roll ロール角
        /// @param pitch ピッチ角
        /// @param yaw ヨー角
        constexpr Euler(ValueType roll, ValueType pitch, ValueType yaw) noexcept
            : roll(roll)
            , pitch(pitch)
            , yaw(yaw)
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        Euler(const Euler&) = default;

        /// @brief デフォルトコピー代入演算子
        Euler& operator=(const Euler&) = default;

        constexpr Euler operator+(const Euler& rhs) const noexcept { return { roll + rhs.roll, pitch + rhs.pitch, yaw + rhs.yaw }; }
        constexpr Euler operator-(const Euler& rhs) const noexcept { return { roll - rhs.roll, pitch - rhs.pitch, yaw - rhs.yaw }; }
        constexpr Euler operator*(const Euler& rhs) const noexcept { return { roll * rhs.roll, pitch * rhs.pitch, yaw * rhs.yaw }; }
        constexpr Euler operator/(const Euler& rhs) const noexcept { return { roll / rhs.roll, pitch / rhs.pitch, yaw / rhs.yaw }; }
        constexpr Euler operator+(ValueType rhs) const noexcept { return { roll + rhs, pitch + rhs, yaw + rhs }; }
        constexpr Euler operator-(ValueType rhs) const noexcept { return { roll - rhs, pitch - rhs, yaw - rhs }; }
        constexpr Euler operator*(ValueType rhs) const noexcept { return { roll * rhs, pitch * rhs, yaw * rhs }; }
        constexpr Euler operator/(ValueType rhs) const noexcept { return { roll / rhs, pitch / rhs, yaw / rhs }; }

        Euler& operator+=(const Euler& rhs) noexcept { return *this = *this + rhs; };
        Euler& operator-=(const Euler& rhs) noexcept { return *this = *this - rhs; };
        Euler& operator*=(const Euler& rhs) noexcept { return *this = *this * rhs; };
        Euler& operator/=(const Euler& rhs) noexcept { return *this = *this / rhs; };
        Euler& operator+=(ValueType rhs) noexcept { return *this = *this + rhs; };
        Euler& operator-=(ValueType rhs) noexcept { return *this = *this - rhs; };
        Euler& operator*=(ValueType rhs) noexcept { return *this = *this * rhs; };
        Euler& operator/=(ValueType rhs) noexcept { return *this = *this / rhs; };

        constexpr bool operator==(const Euler& rhs) const noexcept
        {
            return roll == rhs.roll &&
                   pitch == rhs.pitch &&
                   yaw == rhs.yaw;
        };
        constexpr bool operator!=(const Euler& rhs) const noexcept
        {
            return !(*this == rhs);
        };

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return roll || pitch || yaw;
        }

        /// @brief 要素がゼロであるか返す
        constexpr bool isZero() const noexcept
        {
            return !operator bool();
        }

        /// @brief 正規化されたオイラー角を取得する
        /// @param min
        /// @param max
        /// @return
        Euler normalized(ValueType min, ValueType max) const
        {
            return {
                Udon::Normalized(roll, min, max),
                Udon::Normalized(pitch, min, max),
                Udon::Normalized(yaw, min, max),
            };
        }

        /// @brief 回転方向を修正したオイラー角を取得する
        /// @param direction 回転方向
        /// @return 修正後のオイラー角
        constexpr Euler directionRevision(const EulerDirection& direction) const noexcept
        {
            return {
                roll * (direction.roll ? 1 : -1),
                pitch * (direction.pitch ? 1 : -1),
                yaw * (direction.yaw ? 1 : -1),
            };
        }

        /// @brief クォータニオンに変換する
        /// @return クォータニオン
        Quaternion toQuaternion() const noexcept;

        /// @brief 値クリア
        void clear() noexcept
        {
            *this = {};
        }

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const noexcept
        {
            Serial.print(F("r: ")), Serial.print(roll), Serial.print('\t');
            Serial.print(F("p: ")), Serial.print(pitch), Serial.print('\t');
            Serial.print(F("y: ")), Serial.print(yaw), Serial.print('\t');
        }
#endif

        UDON_ENUMERABLE(roll, pitch, yaw);
    };

}    // namespace Udon
