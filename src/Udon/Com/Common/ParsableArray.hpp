#pragma once

#include <Udon/Com/Serialization.hpp>

namespace Udon
{
    template <typename Message, size_t N>
    struct ParsableArray
    {
        Message  data[N];

        Message& at(size_t i)
        {
            return data[i];
        }
        const Message& at(size_t i) const
        {
            return at(i);
        }

        UDON_PARSABLE(data);
    };

}    // namespace Udon