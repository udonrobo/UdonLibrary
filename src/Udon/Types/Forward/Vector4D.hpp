//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    四次元ベクトル 前方定義
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Algorithm/Math.hpp>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    struct Vector2D;

    struct Vector3D;

    struct Vector4D
    {

        /// @brief 要素の型
        using ValueType = double;

        /// @brief 要素
        ValueType x;
        ValueType y;
        ValueType z;
        ValueType w;

        /// @brief 次元数
        static constexpr size_t Dimension = 4;

        /// @brief デフォルトコンストラクタ
        constexpr Vector4D() noexcept
            : x()
            , y()
            , z()
            , w()
        {
        }

        /// @brief コンストラクタ
        /// @param x x成分
        /// @param y y成分
        constexpr Vector4D(ValueType x, ValueType y, ValueType z, ValueType w) noexcept
            : x(x)
            , y(y)
            , z(z)
            , w(w)
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        Vector4D(const Vector4D&) = default;

        /// @brief デフォルトコピー代入演算子
        Vector4D& operator=(const Vector4D&) = default;

        /// @brief 算術演算子
        /// @note vector [operator] vector
        friend constexpr Vector4D operator+(const Vector4D& lhs, const Vector4D& rhs) noexcept { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w }; }
        friend constexpr Vector4D operator-(const Vector4D& lhs, const Vector4D& rhs) noexcept { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w }; }
        friend constexpr Vector4D operator*(const Vector4D& lhs, const Vector4D& rhs) noexcept { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w }; }
        friend constexpr Vector4D operator/(const Vector4D& lhs, const Vector4D& rhs) noexcept { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w }; }

        /// @brief 算術演算子
        /// @note vector [operator] scalar
        friend constexpr Vector4D operator+(const Vector4D& lhs, ValueType rhs) noexcept { return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs }; }
        friend constexpr Vector4D operator-(const Vector4D& lhs, ValueType rhs) noexcept { return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs }; }
        friend constexpr Vector4D operator*(const Vector4D& lhs, ValueType rhs) noexcept { return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs }; }
        friend constexpr Vector4D operator/(const Vector4D& lhs, ValueType rhs) noexcept { return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs }; }

        /// @brief 算術演算子
        /// @note scalar [operator] vector
        friend constexpr Vector4D operator+(ValueType lhs, const Vector4D& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w }; }
        friend constexpr Vector4D operator-(ValueType lhs, const Vector4D& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w }; }
        friend constexpr Vector4D operator*(ValueType lhs, const Vector4D& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w }; }
        friend constexpr Vector4D operator/(ValueType lhs, const Vector4D& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w }; }

        /// @brief 複合代入演算子
        /// @note vector = vector [operator] vector
        Vector4D& operator+=(const Vector4D& rhs) noexcept { return *this = *this + rhs; };
        Vector4D& operator-=(const Vector4D& rhs) noexcept { return *this = *this - rhs; };
        Vector4D& operator*=(const Vector4D& rhs) noexcept { return *this = *this * rhs; };
        Vector4D& operator/=(const Vector4D& rhs) noexcept { return *this = *this / rhs; };

        /// @brief 複合代入演算子
        /// @note vector = vector [operator] scalar
        Vector4D& operator+=(ValueType rhs) noexcept { return *this = *this + rhs; };
        Vector4D& operator-=(ValueType rhs) noexcept { return *this = *this - rhs; };
        Vector4D& operator*=(ValueType rhs) noexcept { return *this = *this * rhs; };
        Vector4D& operator/=(ValueType rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @note vector [operator] vector
        friend constexpr bool operator==(const Vector4D& lhs, const Vector4D& rhs) noexcept { return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z and lhs.w == rhs.w; }
        friend constexpr bool operator!=(const Vector4D& lhs, const Vector4D& rhs) noexcept { return !(lhs == rhs); }

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return x || y || z || w;
        }

        /// @brief ゼロベクトルであるかを返す
        constexpr bool isZero() const noexcept
        {
            return !operator bool();
        }

        /// @brief 値クリア
        void clear() noexcept
        {
            *this = {};
        }

        Udon::Vector2D xy() const;

        Udon::Vector3D xyz() const;

#ifdef SIV3D_INCLUDED    /// OpenSiv3D との連携

        /// @brief s3d::Vec2からの変換コンストラクタ
        /// @param v s3d::Vec2
        template <typename T>
        constexpr Vector4D(const s3d::Vector4D<T>& v) noexcept
            : x(static_cast<double>(v.x))
            , y(static_cast<double>(v.y))
            , y(static_cast<double>(v.z))
            , y(static_cast<double>(v.w))
        {
        }

        /// @brief Siv3Dのベクトルに変換する
        template <typename T>
        [[nodiscard]]
        s3d::Vector4D<T> asSivVec4() const noexcept
        {
            return {
                static_cast<T>(x),
                static_cast<T>(y),
                static_cast<T>(z),
                static_cast<T>(w),
            };
        }

        /// @brief Siv3Dのベクトルに変換する
        template <typename T>
        [[nodiscard]]
        operator s3d::Vector4D<T>() const noexcept
        {
            return asSivVec4<T>();
        }
#endif

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            Serial.print(F("x: ")), Serial.print(x), Serial.print('\t');
            Serial.print(F("y: ")), Serial.print(y), Serial.print('\t');
            Serial.print(F("z: ")), Serial.print(z), Serial.print('\t');
            Serial.print(F("w: ")), Serial.print(w), Serial.print('\t');
        }
#endif

        UDON_ENUMERABLE(x, y, z, w);
    };

}    // namespace Udon
