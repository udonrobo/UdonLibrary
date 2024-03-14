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
//    二次元ベクトル 前方定義
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Algorithm/Math.hpp>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    struct Polar;

    struct Vector3D;

    struct Vector4D;

    struct Vector2D
    {

        /// @brief 要素の型
        using ValueType = double;

        /// @brief X成分
        ValueType x;

        /// @brief Y成分
        ValueType y;

        /// @brief 次元数
        static constexpr size_t Dimension = 2;

        /// @brief デフォルトコンストラクタ
        constexpr Vector2D() noexcept
            : x()
            , y()
        {
        }

        /// @brief コンストラクタ
        /// @param x x成分
        /// @param y y成分
        constexpr Vector2D(ValueType x, ValueType y) noexcept
            : x(x)
            , y(y)
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        Vector2D(const Vector2D&) = default;

        /// @brief デフォルトコピー代入演算子
        Vector2D& operator=(const Vector2D&) = default;

        /// @brief 算術演算子
        /// @note vector [operator] vector
        friend constexpr Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs) noexcept { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
        friend constexpr Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs) noexcept { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
        friend constexpr Vector2D operator*(const Vector2D& lhs, const Vector2D& rhs) noexcept { return { lhs.x * rhs.x, lhs.y * rhs.y }; }
        friend constexpr Vector2D operator/(const Vector2D& lhs, const Vector2D& rhs) noexcept { return { lhs.x / rhs.x, lhs.y / rhs.y }; }

        /// @brief 算術演算子
        /// @note vector [operator] scalar
        friend constexpr Vector2D operator+(const Vector2D& lhs, ValueType rhs) noexcept { return { lhs.x + rhs, lhs.y + rhs }; }
        friend constexpr Vector2D operator-(const Vector2D& lhs, ValueType rhs) noexcept { return { lhs.x - rhs, lhs.y - rhs }; }
        friend constexpr Vector2D operator*(const Vector2D& lhs, ValueType rhs) noexcept { return { lhs.x * rhs, lhs.y * rhs }; }
        friend constexpr Vector2D operator/(const Vector2D& lhs, ValueType rhs) noexcept { return { lhs.x / rhs, lhs.y / rhs }; }

        /// @brief 算術演算子
        /// @note scalar [operator] vector
        friend constexpr Vector2D operator+(ValueType lhs, const Vector2D& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.x }; }
        friend constexpr Vector2D operator-(ValueType lhs, const Vector2D& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.x }; }
        friend constexpr Vector2D operator*(ValueType lhs, const Vector2D& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.x }; }
        friend constexpr Vector2D operator/(ValueType lhs, const Vector2D& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.x }; }

        /// @brief 複合代入演算子
        /// @note vector = vector [operator] vector
        Vector2D& operator+=(const Vector2D& rhs) noexcept { return *this = *this + rhs; };
        Vector2D& operator-=(const Vector2D& rhs) noexcept { return *this = *this - rhs; };
        Vector2D& operator*=(const Vector2D& rhs) noexcept { return *this = *this * rhs; };
        Vector2D& operator/=(const Vector2D& rhs) noexcept { return *this = *this / rhs; };

        /// @brief 複合代入演算子
        /// @note vector = vector [operator] scalar
        Vector2D& operator+=(ValueType rhs) noexcept { return *this = *this + rhs; };
        Vector2D& operator-=(ValueType rhs) noexcept { return *this = *this - rhs; };
        Vector2D& operator*=(ValueType rhs) noexcept { return *this = *this * rhs; };
        Vector2D& operator/=(ValueType rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @note vector [operator] vector
        friend constexpr bool operator==(const Vector2D& lhs, const Vector2D& rhs) noexcept { return lhs.x == rhs.x and lhs.y == rhs.y; };
        friend constexpr bool operator!=(const Vector2D& lhs, const Vector2D& rhs) noexcept { return not(lhs == rhs); };

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return x or y;
        }

        /// @brief ゼロベクトルを返す
        /// @return ゼロベクトル
        static constexpr Vector2D Zero() noexcept
        {
            return { 0, 0 };
        }

        /// @brief ゼロベクトルであるかを返す
        constexpr bool isZero() const noexcept
        {
            return not operator bool();
        }

        /// @brief 値クリア
        void clear() noexcept
        {
            *this = Zero();
        }

        Vector2D& replaceX(ValueType newValue) noexcept
        {
            x = newValue;
            return *this;
        }

        Vector2D& replaceY(ValueType newValue) noexcept
        {
            y = newValue;
            return *this;
        }

        template <typename Visitor>
        Vector2D& replaceX(Visitor&& visitor) noexcept
        {
            x = visitor(x);
            return *this;
        }

        template <typename Visitor>
        Vector2D& replaceY(Visitor&& visitor) noexcept
        {
            y = visitor(y);
            return *this;
        }

        /// @brief 指定された点を中心に時計回りに回転したベクトルを返す
        /// @param center 回転の中心
        /// @param angle 回転する角度 [rad]
        /// @return 回転後の座標
        Vector2D rotatedAt(const Vector2D& center, ValueType angle) const noexcept
        {
            const auto s = sin(-angle);
            const auto c = cos(-angle);
            const auto d = (*this - center);
            return center + Vector2D{ (d.x * c - d.y * s), (d.x * s + d.y * c) };
        }

        /// @brief 指定された点を中心に時計回りに回転させる
        /// @param center 回転の中心
        /// @param angle 回転する角度 [rad]
        /// @return 回転後の座標
        Vector2D& rotateAt(const Vector2D& center, ValueType angle) noexcept
        {
            return *this = rotatedAt(center, angle);
        }

        /// @brief 原点を中心に回転したベクトルを返す
        /// @param angle 回転する角度 [rad]
        /// @return 回転後の座標
        Vector2D rotated(ValueType angle) const noexcept
        {
            return rotatedAt(Zero(), angle);
        }

        /// @brief 原点を中心に回転させる
        /// @param angle 回転する角度 [rad]
        /// @return 回転後の座標
        Vector2D& rotate(ValueType angle) noexcept
        {
            return *this = rotated(angle);
        }

        /// @brief 指定された点からの角度を求める
        /// @note y軸の正が 0rad
        /// @param rhs 指定された点
        /// @return
        ValueType angleAt(const Vector2D& rhs) const noexcept
        {
            if (*this)
            {
                const auto d = *this - rhs;
                return atan2(d.x, d.y);
            }
            else
            {
                return 0.;
            }
        }

        /// @brief 原点からの時計回りの角度を求める
        /// @note y軸の正が 0rad
        /// @return
        ValueType angle() const noexcept
        {
            return angleAt(Zero());
        }

        /// @brief 指定された点からの距離を求める
        /// @param rhs 指定された点
        /// @return 指定された点からの距離
        ValueType distanceFrom(const Vector2D& rhs) const noexcept
        {
            const auto d = *this - rhs;
            return sqrt(d.x * d.x + d.y * d.y);
        }

        /// @brief 原点からの距離を求める
        /// @return 原点からの距離
        ValueType length() const noexcept
        {
            return distanceFrom(Zero());
        }

        /// @brief 原点からの距離の二乗を求める
        /// @return 原点からの距離の二乗
        constexpr ValueType lengthSq() const noexcept
        {
            return x * x + y * y;
        }

        /// @brief 長さを変更したベクトルを返す
        /// @param length 指定された長さ
        Vector2D scaledLength(ValueType length) const noexcept
        {
            return normalized() * length;
        }

        /// @brief ベクトルの長さを指定された値にする
        /// @note 長さを0にするとゼロベクトルになるので、長さの変更ができなくなる
        /// @param length 指定された長さ
        Vector2D& scaleLength(ValueType length) noexcept
        {
            return *this = scaledLength(length);
        }

        /// @brief 各要素をある範囲から別の範囲に再マップしたベクトルを返す
        /// @param fromMin 元の値の下限
        /// @param fromMax 元の値の上限
        /// @param toMin   再マップ後の値の下限
        /// @param toMax   再マップ後の値の上限
        /// @return
        constexpr Vector2D mapped(ValueType fromMin, ValueType fromMax, ValueType toMin, ValueType toMax) const noexcept
        {
            return {
                Map(x, fromMin, fromMax, toMin, toMax),
                Map(y, fromMin, fromMax, toMin, toMax),
            };
        }

        /// @brief 各要素をある範囲から別の範囲に再マップする
        /// @param fromMin 元の値の下限
        /// @param fromMax 元の値の上限
        /// @param toMin   再マップ後の値の下限
        /// @param toMax   再マップ後の値の上限
        /// @return
        Vector2D& map(ValueType fromMin, ValueType fromMax, ValueType toMin, ValueType toMax) noexcept
        {
            return *this = mapped(fromMin, fromMax, toMin, toMax);
        }

        /// @brief 正規化したベクトルを返す
        /// @return 正規化したベクトル
        Vector2D normalized() const noexcept
        {
            if (const auto len = length())
            {
                return *this / len;
            }
            else
            {
                return Zero();
            }
        }

        /// @brief ベクトルを正規化する
        /// @return
        Vector2D& normalize() noexcept
        {
            return *this = normalized();
        }

        /// @brief 各要素に制限をかけたベクトルを返す
        /// @param min 最小値
        /// @param max 最大値
        /// @return
        constexpr Vector2D clamped(ValueType min, ValueType max) const noexcept
        {
            return { Udon::Constrain(x, min, max), Udon::Constrain(y, min, max) };
        }

        /// @brief 各要素に制限をかける
        /// @param min 最小値
        /// @param max 最大値
        /// @return
        Vector2D& clamp(ValueType min, ValueType max) noexcept
        {
            return *this = clamped(min, max);
        }

        Udon::Vector3D xy0() const noexcept;

        Udon::Vector4D xy00() const noexcept;

        Udon::Polar toPolar() const noexcept;

#ifdef SIV3D_INCLUDED    /// OpenSiv3D との連携

        /// @brief s3d::Vec2からの変換コンストラクタ
        /// @param v s3d::Vec2
        template <typename T>
        constexpr Vector2D(const s3d::Vector2D<T>& v) noexcept
            : x(static_cast<ValueType>(v.x))
            , y(static_cast<ValueType>(v.y))
        {
        }

        /// @brief Siv3Dのベクトルに変換する
        template <typename T>
        [[nodiscard]] s3d::Vector2D<T> asSivVec2() const noexcept
        {
            return {
                static_cast<T>(x),
                static_cast<T>(y),
            };
        }

        /// @brief Siv3Dのベクトルに変換する
        template <typename T>
        [[nodiscard]]
        operator s3d::Vector2D<T>() const noexcept
        {
            return asSivVec2<T>();
        }
#endif

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            Serial.print(F("x: ")), Serial.print(x), Serial.print('\t');
            Serial.print(F("y: ")), Serial.print(y), Serial.print('\t');
        }
#endif

        UDON_ENUMERABLE(x, y);
    };

}    // namespace Udon