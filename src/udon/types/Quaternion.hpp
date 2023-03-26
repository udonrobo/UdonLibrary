#pragma once

#ifdef UDON_ENABLE_STL
#   include <ostream>
#endif

#include <udon\com\serializer\Serializer.hpp>

namespace udon
{
    struct Quaternion
    {

        double w;
        double x;
        double y;
        double z;

        /// @brief メンバイテレーション演算子
        /// @tparam MIterator
        /// @param mit
        /// @param rhs
        /// @return
        template<class MIterator>
        friend MIterator& operator|(MIterator& mit, udon::Quaternion& rhs)
        {
            return mit
                | rhs.w
                | rhs.x
                | rhs.y
                | rhs.z;
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