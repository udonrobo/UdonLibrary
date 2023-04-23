#pragma once

#ifdef UDON_ENABLE_STL
#   include <ostream>
#endif

#include <udon/com/serializer/Serializer.hpp>

namespace udon
{
    struct Quaternion
    {

        double x;
        double y;
        double z;
        double w;

        /// @brief シリアライズ後のバイト数を求める
        /// @return
        constexpr size_t capacity() const
        {
            return udon::Capacity(x, y, z, w);
        }

        /// @brief
        /// @tparam T
        /// @param acc
        template <typename Acc>
        void accessor(Acc& acc)
        {
            acc(x, y, z, w);
        }
    };
}    // namespace udon