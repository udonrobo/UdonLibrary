#pragma once

namespace Udon
{
    template <typename MinT, typename MaxT = MinT>
    struct Range
    {
        MinT min;
        MaxT max;
    };
}    // namespace Udon
