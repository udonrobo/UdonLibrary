//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    色空間
//
//-------------------------------------------------------------------


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
