#pragma once

#if __has_include(<Arduino.h>)

#    include <Arduino.h>

#    include <udon/types/HSV.hpp>
#    include <udon/types/RGB.hpp>

namespace udon
{
    inline RGB Rainbow() noexcept
    {
        return HSV{ static_cast<uint8_t>(millis() / 10.), 255, 255 }.toRGB();
    }
}    // namespace udon

#endif