//
//    色空間
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include "HSV.hpp"
#include "RGB.hpp"

namespace Udon
{
#ifdef ARDUINO

    /// @brief 時間に応じて虹色に変化する色を取得する
    /// @param cycleMs 周期[ms]
    /// @return 虹色
    inline RGB Rainbow(uint32_t cycleMs) noexcept
    {
        const uint32_t hue = Millis() % cycleMs * 255 / cycleMs;
        return HSV{ hue, 255, 255 };
    }
#endif
}    // namespace Udon
