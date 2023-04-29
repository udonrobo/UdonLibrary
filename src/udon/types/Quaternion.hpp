#pragma once

#ifdef UDON_ENABLE_STL
#   include <ostream>
#endif

#include <udon/com/serializer/Serializer.hpp>

namespace udon
{
    struct Quaternion
    {

        /// @brief 要素の型
        using value_type = double;

        value_type x;
        value_type y;
        value_type z;
        value_type w;

        /// @brief デフォルトコンストラクタ
        constexpr Quaternion() noexcept
            : x()
            , y()
            , z()
			, w()
        {}

        /// @brief デフォルトコピーコンストラクタ
        constexpr Quaternion(const Quaternion& rhs) noexcept
            : x(rhs.x)
            , y(rhs.y)
            , z(rhs.z)
			, w(rhs.w)
        {}

        /// @brief コンストラクタ
        /// @param x x成分
        /// @param y y成分
        constexpr Quaternion(value_type x, value_type y, value_type z, value_type w) noexcept
            : x(x)
            , y(y)
            , z(z)
			, w(w)
        {
        }

        /// @brief デフォルトコピー代入演算子
        Quaternion& operator=(const Quaternion&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Quaternion operator+(const Quaternion& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
        constexpr Quaternion operator-(const Quaternion& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
        constexpr Quaternion operator*(const Quaternion& rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
        constexpr Quaternion operator/(const Quaternion& rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }
        constexpr Quaternion operator+(value_type rhs) const noexcept { return { x + rhs, y + rhs, z + rhs, w + rhs  }; }
        constexpr Quaternion operator-(value_type rhs) const noexcept { return { x - rhs, y - rhs, z - rhs, w - rhs  }; }
        constexpr Quaternion operator*(value_type rhs) const noexcept { return { x * rhs, y * rhs, z * rhs, w * rhs  }; }
        constexpr Quaternion operator/(value_type rhs) const noexcept { return { x / rhs, y / rhs, z / rhs, w / rhs  }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        Quaternion& operator+=(const Quaternion& rhs) noexcept { return *this = *this + rhs; };
        Quaternion& operator-=(const Quaternion& rhs) noexcept { return *this = *this - rhs; };
        Quaternion& operator*=(const Quaternion& rhs) noexcept { return *this = *this * rhs; };
        Quaternion& operator/=(const Quaternion& rhs) noexcept { return *this = *this / rhs; };
        Quaternion& operator+=(value_type rhs) noexcept { return *this = *this + rhs; };
        Quaternion& operator-=(value_type rhs) noexcept { return *this = *this - rhs; };
        Quaternion& operator*=(value_type rhs) noexcept { return *this = *this * rhs; };
        Quaternion& operator/=(value_type rhs) noexcept { return *this = *this / rhs; };

        /// @brief 比較演算子
        /// @param rhs 被演算子
        /// @return
        constexpr bool operator==(const Quaternion& rhs) const noexcept
        {
            return x == rhs.x &&
                   y == rhs.y &&
                   z == rhs.z &&
                   w == rhs.w;
        };
        constexpr bool operator!=(const Quaternion& rhs) const noexcept
        {
            return !(*this == rhs);
        };

        /// @brief 要素のいずれかに0以外の値があるかどうかを返す
        /// @return
        explicit constexpr operator bool() const noexcept
        {
            return x || y || z || w;
        }

        /// @brief 要素がゼロであるかを返す
        constexpr bool isZero() const noexcept
        {
            return !operator bool();
        }

        /// @brief 値クリア
        void clear() noexcept
        {
            *this = {};
        }

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

        /// @brief シリアライズ後のバイト数を求める
        /// @return
        constexpr size_t capacity() const
        {
            return udon::Capacity(x, y, z, w);
        }

        /// @brief
        /// @tparam T
        /// @param acc
        template <typename Acc>
        void accessor(Acc& acc)
        {
            acc(x, y, z, w);
        }

    };
}    // namespace udon