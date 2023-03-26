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
