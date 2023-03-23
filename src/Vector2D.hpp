#pragma once

#include <Serializer.hpp>

namespace udon
{
	struct Vec2{
        double x, y;



        friend Serializer& operator<<(Serializer& builder, const Vec2& rhs)
        {
            return builder << rhs.x << rhs.y ;
        }
        friend Serializer& operator>>(Serializer& builder, Vec2& rhs)
        {
            return builder >> rhs.x >> rhs.y;
        }

    };
} // namespace udon
