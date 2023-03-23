#pragma once

#include <com\serializer\Serializer.hpp>

namespace udon
{

	template<class Ty>
	struct Vector3D;

	template<class Ty>
	struct Vector4D;

	template<class Ty>
	struct Vector2D
	{

		using value_type = Ty;

		static constexpr size_t Dimension = 2;

		value_type x;
		value_type y;

		Vector3D<value_type> xy0() const;

		Vector4D<value_type> xy00() const;

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
