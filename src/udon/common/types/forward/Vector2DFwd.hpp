#pragma once

#include <udon\com\serializer\Serializer.hpp>

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

		/// @brief コンストラクタ
		/// @param x x成分
		/// @param y y成分
		Vector2D(value_type x, value_type y)
			: x(x)
			, y(y)
		{}

		/// @brief 変換コンストラクタ
		/// @param x x成分
		/// @param y y成分
		Vector2D(value_type rhs)
			: x(rhs)
			, y(rhs)
		{}

	    /// @brief デフォルトコピー代入演算子
	    Vector2D& operator=(const Vector2D&) = default;

	    /// @brief 算術演算子
	    /// @param rhs 被演算子
	    /// @return
	    Vector2D operator+(const Vector2D& rhs) const { return { x + rhs.x, y + rhs.y }; }
	    Vector2D operator-(const Vector2D& rhs) const { return { x - rhs.x, y - rhs.y }; }
	    Vector2D operator*(const Vector2D& rhs) const { return { x * rhs.x, y * rhs.y }; }
	    Vector2D operator/(const Vector2D& rhs) const { return { x / rhs.x, y / rhs.y }; }

	    /// @brief 複合代入演算子
	    /// @param rhs 被演算子
	    /// @return
	    Vector2D& operator+=(const Vector2D& rhs) { return *this = *this + rhs; };
	    Vector2D& operator-=(const Vector2D& rhs) { return *this = *this - rhs; };
	    Vector2D& operator*=(const Vector2D& rhs) { return *this = *this * rhs; };
	    Vector2D& operator/=(const Vector2D& rhs) { return *this = *this / rhs; };

		/// @brief どちらかの要素が0でないかどうかを返す
		explicit operator bool() const
		{
			return x || y;
		}

		/// @brief 値クリア
		void clear()
		{
			*this = {};
		}

		/// @brief 指定された点を中心に回転
		/// @param center 回転の中心
		/// @param angle 回転する角度 [rad]
		/// @return 回転後の座標
		Vector2D rotatedAt(const Vector2D& center, double angle) const
		{
			const auto s = sin(-angle);
			const auto c = cos(-angle);
			const auto d = (*this - center);
			return center + Vector2D { (d.x * c - d.y * s), (d.x * s + d.y * c) };
		}

		/// @brief 原点を中心に回転
		/// @param angle 回転する角度 [rad]
		/// @return 回転後の座標
		Vector2D rotated(double angle) const
		{
			return rotatedAt({ 0, 0 }, angle);
		}

		/// @brief 指定された点からの角度を求める
		/// @remark y軸の正が 0rad
		/// @param rhs 指定された点
		/// @return
		value_type angleAt(const Vector2D& rhs) const
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
		value_type distanceFrom(const Vector2D& rhs) const
		{
			const auto d = *this - rhs;
			return sqrt(sq(d.x) + sq(d.y));
		}

		/// @brief 原点からの距離を求める
		/// @return 原点からの距離
		value_type length() const
		{
			return distanceFrom({ 0, 0 });
		}

		Vector3D<value_type> xy0() const;

		Vector4D<value_type> xy00() const;

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
