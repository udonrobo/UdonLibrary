#pragma once

#ifdef ARDUINO_ARCH_RP2040

namespace Udon
{
    class PicoWDT
    {
    public:
        PicoWDT(uint32_t ms = 1000)
        {
            rp2040.wdt_begin(ms);
        }

        void update()
        {
            rp2040.wdt_reset();
        }
    };

}    // namespace Udon

#endif
