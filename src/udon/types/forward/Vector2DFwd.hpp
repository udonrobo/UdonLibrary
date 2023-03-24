#pragma once

#include <udon\com\serializer\Serializer.hpp>
#include <udon\math\Math.hpp>

namespace udon
{

	template<class Ty>
	struct Vector3D;

	template<class Ty>
	struct Vector4D;

	template<class Ty>
	struct Vector2D
	{

		/// @brief 要素の型
		using value_type = Ty;

		/// @brief 次元数
		static constexpr size_t Dimension = 2;

		/// @brief x成分
		value_type x;

		/// @brief y成分
		value_type y;

		/// @brief デフォルトコンストラクタ
		Vector2D() = default;

		/// @brief デフォルトコピーコンストラクタ
		Vector2D(const Vector2D& rhs) = default;

		/// @brief コンストラクタ
		/// @param x x成分
		/// @param y y成分
		constexpr Vector2D(value_type x, value_type y)
			: x(x)
			, y(y)
		{}

		/// @brief 変換コンストラクタ
		/// @param rhs
		constexpr Vector2D(value_type rhs)
			: x(rhs)
			, y(rhs)
		{}

	    /// @brief デフォルトコピー代入演算子
	    Vector2D& operator=(const Vector2D&) = default;

	    /// @brief 算術演算子
	    /// @param rhs 被演算子
	    /// @return
	    constexpr Vector2D operator+(const Vector2D& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
	    constexpr Vector2D operator-(const Vector2D& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
	    constexpr Vector2D operator*(const Vector2D& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
	    constexpr Vector2D operator/(const Vector2D& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }

	    /// @brief 複合代入演算子
	    /// @param rhs 被演算子
	    /// @return
	    constexpr Vector2D& operator+=(const Vector2D& rhs) noexcept { return *this = *this + rhs; };
	    constexpr Vector2D& operator-=(const Vector2D& rhs) noexcept { return *this = *this - rhs; };
	    constexpr Vector2D& operator*=(const Vector2D& rhs) noexcept { return *this = *this * rhs; };
	    constexpr Vector2D& operator/=(const Vector2D& rhs) noexcept { return *this = *this / rhs; };

		/// @brief 比較演算子
		/// @param rhs 被演算子
		/// @return
		constexpr bool operator==(const Vector2D& rhs) const noexcept
		{
			return
				x == rhs.x &&
				y == rhs.y;
		};
		constexpr bool operator!=(const Vector2D& rhs) const noexcept
		{
			return !(*this == rhs);
		};

		/// @brief ゼロベクトルであるかを返す
		constexpr bool isZero() const noexcept
		{
			return (x || y);
		}

		/// @brief 値クリア
		constexpr void clear() noexcept
		{
			*this = {};
		}

		/// @brief 指定された点を中心に回転
		/// @param center 回転の中心
		/// @param angle 回転する角度 [rad]
		/// @return 回転後の座標
		Vector2D rotatedAt(const Vector2D& center, value_type angle) const noexcept
		{
			const auto s = sin(-angle);
			const auto c = cos(-angle);
			const auto d = (*this - center);
			return center + Vector2D { (d.x * c - d.y * s), (d.x * s + d.y * c) };
		}

		/// @brief 原点を中心に回転
		/// @param angle 回転する角度 [rad]
		/// @return 回転後の座標
		Vector2D rotated(value_type angle) const noexcept
		{
			return rotatedAt({ 0, 0 }, angle);
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
			return sqrt(udon::Sq(d.x) + udon::Sq(d.y));
		}

		/// @brief 原点からの距離を求める
		/// @return 原点からの距離
		value_type length() const noexcept
		{
			return distanceFrom({ 0, 0 });
		}

		udon::Vector3D<value_type> xy0() const noexcept;

		udon::Vector4D<value_type> xy00() const noexcept;

        /// @brief シリアライザ用オーバーロード
        friend Serializer& operator<<(Serializer& builder, const Vector2D& rhs)
        {
			builder.reserve(sizeof(value_type) * Vector2D::Dimension);
            return builder << rhs.x << rhs.y;
        }
        friend Serializer& operator>>(Serializer& builder, Vector2D& rhs)
        {
            return builder >> rhs.x >> rhs.y;
        }

	};

}
