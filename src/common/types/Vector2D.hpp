#pragma once

#include <UdonFwd.hpp>
#include <com\serializer\Serializer.hpp>

namespace udon
{

    template<class Ty>
	struct Vec2
    {

        double x, y;

        friend Serializer& operator<<(Serializer& builder, const Vec2& rhs)
        {
            return builder << rhs.x << rhs.y;
        }
        friend Serializer& operator>>(Serializer& builder, Vec2& rhs)
        {
            return builder >> rhs.x >> rhs.y;
        }

    };
}
