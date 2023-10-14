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
//    四次元ベクトル 前方定義
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Traits/ParsableMacro.hpp>
#include <Udon/Com/Serialization.hpp>

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
        /// @param rhs 被演算子
        /// @return
        constexpr Vector4D operator+(const Vector4D& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
        constexpr Vector4D operator-(const Vector4D& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
        constexpr Vector4D operator*(const Vector4D& rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
        constexpr Vector4D operator/(const Vector4D& rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }
        constexpr Vector4D operator+(ValueType rhs) const noexcept { return { x + rhs, y + rhs, z + rhs, w + rhs }; }
        constexpr Vector4D operator-(ValueType rhs) const noexcept { return { x - rhs, y - rhs, z - rhs, w - rhs }; }
        constexpr Vector4D operator*(ValueType rhs) const noexcept { return { x * rhs, y * rhs, z * rhs, w * rhs }; }
        constexpr Vector4D operator/(ValueType rhs) const noexcept { return { x / rhs, y / rhs, z / rhs, w / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        Vector4D& operator+=(const Vector4D& rhs) noexcept { return *this = *this + rhs; };
        Vector4D& operator-=(const Vector4D& rhs) noexcept { return *this = *this - rhs; };
        Vector4D& operator*=(const Vector4D& rhs) noexcept { return *this = *this * rhs; };
        Vector4D& operator/=(const Vector4D& rhs) noexcept { return *this = *this / rhs; };
        Vector4D& operator+=(ValueType rhs) noexcept { return *this = *this + rhs; };
        Vector4D& operator-=(ValueType rhs) noexcept { return *this = *this - rhs; };
        Vector4D& operator*=(ValueType rhs) noexcept { return *this = *this * rhs; };
        Vector4D& operator/=(ValueType rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Vector4D& rhs) const noexcept
        {
            return x == rhs.x &&
                   y == rhs.y &&
                   z == rhs.z &&
                   w == rhs.w;
        };
        constexpr bool operator!=(const Vector4D& rhs) const noexcept
        {
            return !(*this == rhs);
        };

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

        UDON_PARSABLE(x, y, z, w);
    };

}    // namespace Udon
