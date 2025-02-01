#pragma once

namespace Udon
{
    /// @brief 範囲を表す型
    template <typename MinT, typename MaxT = MinT>
    struct Range
    {
        MinT min;  ///< 最小値
        MaxT max;  ///< 最大値
    };
}    // namespace Udon
