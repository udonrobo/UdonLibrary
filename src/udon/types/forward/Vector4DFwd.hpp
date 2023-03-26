#pragma once

#include <udon\com\serializer\Serializer.hpp>

namespace udon
{

	template<class Ty>
	struct Vector2D;

	template<class Ty>
	struct Vector3D;

	template<class Ty>
	struct Vector4D
	{

        /// @brief 要素の型
		using value_type = Ty;

		/// @brief 要素
		value_type x;
		value_type y;
		value_type z;
		value_type w;

        /// @brief 次元数
		static constexpr size_t Dimension = 4;

        /// @brief デフォルトコンストラクタ
        constexpr Vector4D() noexcept
            : x()
            , y()
            , z()
			, w()
        {}

        /// @brief デフォルトコピーコンストラクタ
        constexpr Vector4D(const Vector4D& rhs) noexcept
            : x(rhs.x)
            , y(rhs.y)
            , z(rhs.z)
			, w(rhs.w)
        {}

        /// @brief コンストラクタ
        /// @param x x成分
        /// @param y y成分
        constexpr Vector4D(value_type x, value_type y, value_type z, value_type w) noexcept
            : x(x)
            , y(y)
            , z(z)
			, w(w)
        {
        }

        /// @brief デフォルトコピー代入演算子
        Vector4D& operator=(const Vector4D&) = default;

        /// @brief 算術演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Vector4D operator+(const Vector4D& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
        constexpr Vector4D operator-(const Vector4D& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
        constexpr Vector4D operator*(const Vector4D& rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
        constexpr Vector4D operator/(const Vector4D& rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }
        constexpr Vector4D operator+(value_type rhs) const noexcept { return { x + rhs, y + rhs, z + rhs, w + rhs  }; }
        constexpr Vector4D operator-(value_type rhs) const noexcept { return { x - rhs, y - rhs, z - rhs, w - rhs  }; }
        constexpr Vector4D operator*(value_type rhs) const noexcept { return { x * rhs, y * rhs, z * rhs, w * rhs  }; }
        constexpr Vector4D operator/(value_type rhs) const noexcept { return { x / rhs, y / rhs, z / rhs, w / rhs  }; }

        /// @brief 複合代入演算子
        /// @param rhs 被演算子
        /// @return
        constexpr Vector4D& operator+=(const Vector4D& rhs) noexcept { return *this = *this + rhs; };
        constexpr Vector4D& operator-=(const Vector4D& rhs) noexcept { return *this = *this - rhs; };
        constexpr Vector4D& operator*=(const Vector4D& rhs) noexcept { return *this = *this * rhs; };
        constexpr Vector4D& operator/=(const Vector4D& rhs) noexcept { return *this = *this / rhs; };
        constexpr Vector4D& operator+=(value_type rhs) noexcept { return *this = *this + rhs; };
        constexpr Vector4D& operator-=(value_type rhs) noexcept { return *this = *this - rhs; };
        constexpr Vector4D& operator*=(value_type rhs) noexcept { return *this = *this * rhs; };
        constexpr Vector4D& operator/=(value_type rhs) noexcept { return *this = *this / rhs; };

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

		udon::Vector2D<value_type> xy() const;

		udon::Vector3D<value_type> xyz() const;

        /// @brief メンバイテレーション演算子
        /// @tparam MIterator
        /// @param mit
        /// @param rhs
        /// @return
        template<class MIterator>
        friend MIterator& operator|(MIterator& mit, udon::Vector4D<Ty>& rhs)
        {
            return mit
                | rhs.x
                | rhs.y
                | rhs.z
                | rhs.w;
        }

	};

}
