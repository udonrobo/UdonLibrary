#pragma once

#include <com\serializer\Serializer.hpp>

namespace udon
{

	template<class Ty>
	struct Vector2D;

	template<class Ty>
	struct Vector4D;

	template<class Ty>
	struct Vector3D
	{

		using value_type = Ty;

		static constexpr size_t Dimension = 3;

		value_type x;
		value_type y;
		value_type z;

		Vector2D<value_type> xy() const;

		Vector4D<value_type> xyz0() const;

        /// @brief シリアライザ用オーバーロード
        friend Serializer& operator<<(Serializer& builder, const Vector3D& rhs)
        {
			builder.reserve(sizeof(value_type) * Vector3D::Dimension);
            return builder << rhs.x << rhs.y << rhs.z;
        }
        friend Serializer& operator>>(Serializer& builder, Vector3D& rhs)
        {
            return builder >> rhs.x >> rhs.y >> rhs.z;
        }

	};

}
