//
//    色空間
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include "HSV.hpp"
#include "RGB.hpp"

namespace Udon
{
#ifdef ARDUINO
    inline RGB Rainbow() noexcept
    {
        return HSV{ static_cast<uint8_t>(millis() / 10.), 255, 255 }.toRGB();
    }
#endif
}    // namespace Udon
