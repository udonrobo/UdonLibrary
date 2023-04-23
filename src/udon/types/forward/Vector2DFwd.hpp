#pragma once

#include <udon/com/serializer/Serializer.hpp>

namespace udon
{

    template <class Ty>
    struct Vector3D;

    template <class Ty>
    struct Vector4D;

    template <class Ty>
    struct Vector2D
    {

        /// @brief 要素の型
        using value_type = Ty;

        /// @brief 成分
        value_type x;
        value_type y;

        /// @brief 次元数
        static constexpr size_t Dimension = 2;

        /// @brief デフォルトコンストラクタ
        constexpr Vector2D() noexcept
            : x()
            , y()
        {
        }

        /// @brief デフォルトコピーコンストラクタ
        constexpr Vector2D(const Vector2D& rhs) noexcept
            : x(rhs.x)
            , y(rhs.y)
        {
        }

        /// @brief コンストラクタ
        /// @param x x成分
        /// @param y y成分
        constexpr Vector2D(value_type x, value_type y) noexcept
            : x(x)
            , y(y)
        {
        }

        /// @brief デフォルトコピー代入演算子
        Vector2D& operator=(const Vector2D&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Vector2D operator+(const Vector2D& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
        constexpr Vector2D operator-(const Vector2D& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
        constexpr Vector2D operator*(const Vector2D& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
        constexpr Vector2D operator/(const Vector2D& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }
        constexpr Vector2D operator+(value_type rhs) const noexcept { return { x + rhs, y + rhs }; }
        constexpr Vector2D operator-(value_type rhs) const noexcept { return { x - rhs, y - rhs }; }
        constexpr Vector2D operator*(value_type rhs) const noexcept { return { x * rhs, y * rhs }; }
        constexpr Vector2D operator/(value_type rhs) const noexcept { return { x / rhs, y / rhs }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Vector2D& operator+=(const Vector2D& rhs) noexcept { return *this = *this + rhs; };
        constexpr Vector2D& operator-=(const Vector2D& rhs) noexcept { return *this = *this - rhs; };
        constexpr Vector2D& operator*=(const Vector2D& rhs) noexcept { return *this = *this * rhs; };
        constexpr Vector2D& operator/=(const Vector2D& rhs) noexcept { return *this = *this / rhs; };
        constexpr Vector2D& operator+=(value_type rhs) noexcept { return *this = *this + rhs; };
        constexpr Vector2D& operator-=(value_type rhs) noexcept { return *this = *this - rhs; };
        constexpr Vector2D& operator*=(value_type rhs) noexcept { return *this = *this * rhs; };
        constexpr Vector2D& operator/=(value_type rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Vector2D& rhs) const noexcept
        {
            return x == rhs.x &&
                   y == rhs.y;
        };
        constexpr bool operator!=(const Vector2D& rhs) const noexcept
        {
            return !(*this == rhs);
        };

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return x || y;
        }

        /// @brief ゼロベクトルであるかを返す
        constexpr bool isZero() const noexcept
        {
            return !Vector2D::operator bool();
        }

        /// @brief 値クリア
        void clear() noexcept
        {
            *this = {};
        }

        /// @brief 指定された点を中心に回転したベクトルを返す
        /// @param center 回転の中心
        /// @param angle 回転する角度 [rad]
        /// @return 回転後の座標
        Vector2D rotatedAt(const Vector2D& center, value_type angle) const noexcept
        {
            const auto s = sin(-angle);
            const auto c = cos(-angle);
            const auto d = (*this - center);
            return center + Vector2D{ (d.x * c - d.y * s), (d.x * s + d.y * c) };
        }

        /// @brief 原点を中心に回転したベクトルを返す
        /// @param angle 回転する角度 [rad]
        /// @return 回転後の座標
        Vector2D rotated(value_type angle) const noexcept
        {
            return rotatedAt({ 0, 0 }, angle);
        }

        /// @brief 原点からの角度を求める
        /// @remark y軸の正が 0rad
        /// @return
        value_type angle() const noexcept
        {
            return angleAt({ 0, 0 });
        }

        /// @brief 指定された点からの角度を求める
        /// @remark y軸の正が 0rad
        /// @param rhs 指定された点
        /// @return
        value_type angleAt(const Vector2D& rhs) const noexcept
        {
            if (*this)
            {
                const auto d = *this - rhs;
                return atan2(d.x, d.y);
            }
            else
            {
                return {};
            }
        }

        /// @brief 指定された点からの距離を求める
        /// @param rhs 指定された点
        /// @return 指定された点からの距離
        value_type distanceFrom(const Vector2D& rhs) const noexcept
        {
            const auto d = *this - rhs;
            return sqrt(d.x * d.x + d.y * d.y);
        }

        /// @brief 原点からの距離を求める
        /// @return 原点からの距離
        value_type length() const noexcept
        {
            return distanceFrom({ 0, 0 });
        }

        /// @brief ベクトルの長さを指定された値にする
        /// @param length 指定された長さ
        Vector2D& setLength(value_type length) noexcept
        {
            if (*this)
            {
                if (const auto l = this->length())
                {
                    *this *= length / l;
                }
            }
            return *this;
        }

        udon::Vector3D<value_type> xy0() const noexcept;

        udon::Vector4D<value_type> xy00() const noexcept;

        void show() const
        {
            Serial.print(x), Serial.print('/t');
            Serial.print(y);
        }

        /// @brief シリアライズ後のバイト数を求める
        /// @return
        constexpr size_t capacity() const
        {
            return udon::Capacity(x, y);
        }

        /// @brief
        /// @tparam T
        /// @param acc
        template <typename Acc>
        void accessor(Acc& acc)
        {
            acc(x, y);
        }
    };

}    // namespace udon
