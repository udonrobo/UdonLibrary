#pragma once

#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

    template<typename T>
    struct Vector2D;

    template<typename T>
    struct Vector4D;

    template<typename T>
    struct Vector3D
    {

        /// @brief 要素の型
        using value_type = T;

        /// @brief 要素
        value_type x;
        value_type y;
        value_type z;

        /// @brief 次元数
        static constexpr size_t Dimension = 3;

        /// @brief デフォルトコンストラクタ
        constexpr Vector3D() noexcept
            : x()
            , y()
            , z()
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        constexpr Vector3D(const Vector3D& rhs) noexcept
            : x(rhs.x)
            , y(rhs.y)
            , z(rhs.z)
        {
        }

        /// @brief コンストラクタ
        /// @param x x成分
        /// @param y y成分
        constexpr Vector3D(value_type x, value_type y, value_type z) noexcept
            : x(x)
            , y(y)
            , z(z)
        {
        }

        /// @brief デフォルトコピー代入演算子
        Vector3D& operator=(const Vector3D&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Vector3D operator+(const Vector3D& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
        constexpr Vector3D operator-(const Vector3D& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
        constexpr Vector3D operator*(const Vector3D& rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
        constexpr Vector3D operator/(const Vector3D& rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
        constexpr Vector3D operator+(value_type rhs) const noexcept { return { x + rhs, y + rhs, z + rhs }; }
        constexpr Vector3D operator-(value_type rhs) const noexcept { return { x - rhs, y - rhs, z - rhs }; }
        constexpr Vector3D operator*(value_type rhs) const noexcept { return { x * rhs, y * rhs, z * rhs }; }
        constexpr Vector3D operator/(value_type rhs) const noexcept { return { x / rhs, y / rhs, z / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        Vector3D& operator+=(const Vector3D& rhs) noexcept { return *this = *this + rhs; };
        Vector3D& operator-=(const Vector3D& rhs) noexcept { return *this = *this - rhs; };
        Vector3D& operator*=(const Vector3D& rhs) noexcept { return *this = *this * rhs; };
        Vector3D& operator/=(const Vector3D& rhs) noexcept { return *this = *this / rhs; };
        Vector3D& operator+=(value_type rhs) noexcept { return *this = *this + rhs; };
        Vector3D& operator-=(value_type rhs) noexcept { return *this = *this - rhs; };
        Vector3D& operator*=(value_type rhs) noexcept { return *this = *this * rhs; };
        Vector3D& operator/=(value_type rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Vector3D& rhs) const noexcept
        {
            return x == rhs.x &&
                   y == rhs.y &&
                   z == rhs.z;
        };
        constexpr bool operator!=(const Vector3D& rhs) const noexcept
        {
            return !(*this == rhs);
        };

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return x || y || z;
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

        udon::Vector2D<value_type> xy() const noexcept;

        udon::Vector4D<value_type> xyz0() const noexcept;

#ifdef ARDUINO
        /// @brief デバッグ出力
        void show() const
        {
            Serial.print("x: "), Serial.print(x), Serial.print('\t');
            Serial.print("y: "), Serial.print(y), Serial.print('\t');
            Serial.print("z: "), Serial.print(z), Serial.print('\t');
        }
#endif

        /// @brief シリアライズ後のバイト数を求める
        /// @return
        constexpr size_t capacity() const
        {
            return udon::Capacity(x, y, z);
        }

        /// @brief
        /// @tparam T
        /// @param acc
        template <typename Acc>
        void accessor(Acc& acc)
        {
            acc(x, y, z);
        }
    };

}    // namespace udon
