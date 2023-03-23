#pragma once

#ifdef UDON_ENABLE_STL
#   include <ostream>
#endif

#include <com\serializer\Serializer.hpp>

namespace udon
{
    struct Quaternion
    {

        double i;
        double j;
        double k;
        double w;

        friend Serializer& operator<<(Serializer& builder, const Quaternion& rhs)
        {
            return builder << rhs.i << rhs.j << rhs.k << rhs.w;
        }
        friend Serializer& operator>>(Serializer& builder, Quaternion& rhs)
        {
            return builder >> rhs.i >> rhs.j >> rhs.k >> rhs.w;
        }

#ifdef UDON_ENABLE_STL
        friend std::ostream& operator<<(std::ostream& ostm, const Quaternion& rhs)
        {
            return ostm << "{ " << rhs.i
                        << ", " << rhs.j
                        << ", " << rhs.k
                        << ", " << rhs.w
                        << " }";
        }
#endif
    };
}    // namespace udon