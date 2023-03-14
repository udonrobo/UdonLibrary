#pragma once

#include <cstddef>

namespace udon
{

    /// @brief  配列長を静的に取得
    /// @param  配列
    /// @return 配列長
    template <class Ty, size_t Len>
    inline constexpr size_t Length(const Ty (&)[Len])
    {
        return Len;
    }

}    // namespace udon
