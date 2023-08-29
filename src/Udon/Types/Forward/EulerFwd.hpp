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
//    オイラー角 前方定義
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Math/Math.hpp>    // Udon::Normalized
#include <Udon/Traits/Parsable.hpp>
#include <Udon/Com/Serialization.hpp>

namespace Udon
{

    struct Quaternion;

    template <typename T>
    struct Euler3D
    {

        /// @brief 要素の型
        using value_type = T;

        /// @brief ロール角
        value_type roll;

        /// @brief ピッチ角
        value_type pitch;

        /// @brief ヨー角
        value_type yaw;

        /// @brief デフォルトコンストラクタ
        constexpr Euler3D() noexcept
            : roll()
            , pitch()
            , yaw()
        {
        }

        /// @brief コンストラクタ
        /// @param roll ロール角
        /// @param pitch ピッチ角
        /// @param yaw ヨー角
        constexpr Euler3D(value_type roll, value_type pitch, value_type yaw) noexcept
            : roll(roll)
            , pitch(pitch)
            , yaw(yaw)
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        Euler3D(const Euler3D&) = default;

        /// @brief デフォルトコピー代入演算子
        Euler3D& operator=(const Euler3D&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Euler3D operator+(const Euler3D& rhs) const noexcept { return { roll + rhs.roll, pitch + rhs.pitch, yaw + rhs.yaw }; }
        constexpr Euler3D operator-(const Euler3D& rhs) const noexcept { return { roll - rhs.roll, pitch - rhs.pitch, yaw - rhs.yaw }; }
        constexpr Euler3D operator*(const Euler3D& rhs) const noexcept { return { roll * rhs.roll, pitch * rhs.pitch, yaw * rhs.yaw }; }
        constexpr Euler3D operator/(const Euler3D& rhs) const noexcept { return { roll / rhs.roll, pitch / rhs.pitch, yaw / rhs.yaw }; }
        constexpr Euler3D operator+(value_type rhs) const noexcept { return { roll + rhs, pitch + rhs, yaw + rhs }; }
        constexpr Euler3D operator-(value_type rhs) const noexcept { return { roll - rhs, pitch - rhs, yaw - rhs }; }
        constexpr Euler3D operator*(value_type rhs) const noexcept { return { roll * rhs, pitch * rhs, yaw * rhs }; }
        constexpr Euler3D operator/(value_type rhs) const noexcept { return { roll / rhs, pitch / rhs, yaw / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        Euler3D& operator+=(const Euler3D& rhs) noexcept { return *this = *this + rhs; };
        Euler3D& operator-=(const Euler3D& rhs) noexcept { return *this = *this - rhs; };
        Euler3D& operator*=(const Euler3D& rhs) noexcept { return *this = *this * rhs; };
        Euler3D& operator/=(const Euler3D& rhs) noexcept { return *this = *this / rhs; };
        Euler3D& operator+=(value_type rhs) noexcept { return *this = *this + rhs; };
        Euler3D& operator-=(value_type rhs) noexcept { return *this = *this - rhs; };
        Euler3D& operator*=(value_type rhs) noexcept { return *this = *this * rhs; };
        Euler3D& operator/=(value_type rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Euler3D& rhs) const noexcept
        {
            return roll == rhs.roll &&
                   pitch == rhs.pitch &&
                   yaw == rhs.yaw;
        };
        constexpr bool operator!=(const Euler3D& rhs) const noexcept
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
        Euler3D normalized(value_type min, value_type max) const
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
        Euler3D directionRevision(const Euler3D<bool>& direction) const noexcept
        {
            return {
                roll * (direction.roll ? 1 : -1),
                pitch * (direction.pitch ? 1 : -1),
                yaw * (direction.yaw ? 1 : -1),
            };
        }

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

        UDON_PARSABLE(roll, pitch, yaw);
    };

    using Euler = Euler3D<double>;

}    // namespace Udon
