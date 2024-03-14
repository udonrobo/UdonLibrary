//
//    三次元ベクトル 前方定義
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <Udon/Algorithm/Math.hpp>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{

    struct Vector2D;

    struct Vector3D
    {

        /// @brief 要素の型
        using ValueType = double;

        /// @brief 要素
        ValueType x;
        ValueType y;
        ValueType z;

        /// @brief 次元数
        static constexpr size_t Dimension = 3;

        /// @brief デフォルトコンストラクタ
        constexpr Vector3D() noexcept
            : x()
            , y()
            , z()
        {
        }

        /// @brief コンストラクタ
        /// @param x x成分
        /// @param y y成分
        constexpr Vector3D(ValueType x, ValueType y, ValueType z) noexcept
            : x(x)
            , y(y)
            , z(z)
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        Vector3D(const Vector3D&) = default;

        /// @brief デフォルトコピー代入演算子
        Vector3D& operator=(const Vector3D&) = default;

        /// @brief 算術演算子
        /// @note vector * vector
        friend constexpr Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs) noexcept { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
        friend constexpr Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs) noexcept { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
        friend constexpr Vector3D operator*(const Vector3D& lhs, const Vector3D& rhs) noexcept { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }
        friend constexpr Vector3D operator/(const Vector3D& lhs, const Vector3D& rhs) noexcept { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }

        /// @brief 算術演算子
        /// @note vector * scalar
        friend constexpr Vector3D operator+(const Vector3D& lhs, ValueType rhs) noexcept { return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs }; }
        friend constexpr Vector3D operator-(const Vector3D& lhs, ValueType rhs) noexcept { return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs }; }
        friend constexpr Vector3D operator*(const Vector3D& lhs, ValueType rhs) noexcept { return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs }; }
        friend constexpr Vector3D operator/(const Vector3D& lhs, ValueType rhs) noexcept { return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs }; }

        /// @brief 算術演算子
        /// @note scalar [operator] vector
        friend constexpr Vector3D operator+(ValueType lhs, const Vector3D& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y, lhs + rhs.z }; }
        friend constexpr Vector3D operator-(ValueType lhs, const Vector3D& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y, lhs - rhs.z }; }
        friend constexpr Vector3D operator*(ValueType lhs, const Vector3D& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z }; }
        friend constexpr Vector3D operator/(ValueType lhs, const Vector3D& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z }; }

        /// @brief 複合代入演算子
        /// @note vector = vector [operator] vector
        Vector3D& operator+=(const Vector3D& rhs) noexcept { return *this = *this + rhs; };
        Vector3D& operator-=(const Vector3D& rhs) noexcept { return *this = *this - rhs; };
        Vector3D& operator*=(const Vector3D& rhs) noexcept { return *this = *this * rhs; };
        Vector3D& operator/=(const Vector3D& rhs) noexcept { return *this = *this / rhs; };

        /// @brief 複合代入演算子
        /// @note vector = vector [operator] scalar
        Vector3D& operator+=(ValueType rhs) noexcept { return *this = *this + rhs; };
        Vector3D& operator-=(ValueType rhs) noexcept { return *this = *this - rhs; };
        Vector3D& operator*=(ValueType rhs) noexcept { return *this = *this * rhs; };
        Vector3D& operator/=(ValueType rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @note vector [operator] vector
        friend constexpr bool operator==(const Vector3D& lhs, const Vector3D& rhs) noexcept { return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z; }
        friend constexpr bool operator!=(const Vector3D& lhs, const Vector3D& rhs) noexcept { return not(lhs == rhs); }

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return x || y || z;
        }

        /// @brief ゼロベクトルであるかを返す
        constexpr bool isZero() const noexcept
        {
            return not operator bool();
        }

        /// @brief 値クリア
        void clear() noexcept
        {
            *this = {};
        }

        Udon::Vector2D xy() const noexcept;

#ifdef SIV3D_INCLUDED    /// OpenSiv3D との連携

        /// @brief s3d::Vec2からの変換コンストラクタ
        /// @param v s3d::Vec2
        template <typename T>
        constexpr Vector3D(const s3d::Vector3D<T>& v) noexcept
            : x(static_cast<double>(v.x))
            , y(static_cast<double>(v.y))
            , y(static_cast<double>(v.z))
        {
        }

        /// @brief Siv3Dのベクトルに変換する
        template <typename T>
        [[nodiscard]] s3d::Vector3D<T> asSivVec3() const noexcept
        {
            return {
                static_cast<T>(x),
                static_cast<T>(y),
                static_cast<T>(z),
            };
        }

        /// @brief Siv3Dのベクトルに変換する
        template <typename T>
        [[nodiscard]]
        operator s3d::Vector3D<T>() const noexcept
        {
            return asSivVec3<T>();
        }
#endif

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            Serial.print("x: "), Serial.print(x), Serial.print('\t');
            Serial.print("y: "), Serial.print(y), Serial.print('\t');
            Serial.print("z: "), Serial.print(z), Serial.print('\t');
        }
#endif
        UDON_ENUMERABLE(x, y, z);
    };

}    // namespace Udon
