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

		using value_type = Ty;

		static constexpr size_t Dimension = 4;

		value_type x;
		value_type y;
		value_type z;
		value_type w;

		Vector2D<value_type> xy() const;

		Vector3D<value_type> xyz() const;

        /// @brief シリアライザ用オーバーロード
        friend Serializer& operator<<(Serializer& builder, const Vector4D& rhs)
        {
			builder.reserve(sizeof(value_type) * Vector4D::Dimension);
            return builder << rhs.x << rhs.y << rhs.z << rhs.w;
        }
        friend Serializer& operator>>(Serializer& builder, Vector4D& rhs)
        {
            return builder >> rhs.x >> rhs.y >> rhs.z >> rhs.w;
        }


	};

}
