#pragma once

#include <udon/types/HSV.hpp>
#include <udon/types/RGB.hpp>

namespace udon
{
#ifdef ARDUINO
    inline RGB Rainbow() noexcept
    {
        return HSV{ static_cast<uint8_t>(millis() / 10.), 255, 255 }.toRGB();
    }
#endif
}    // namespace udon
