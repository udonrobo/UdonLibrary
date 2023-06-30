// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#    include "hardware/pio.h"
#    include "pico/stdlib.h"

#endif

// ---------- //
// squarewave //
// ---------- //

namespace Udon
{
    namespace pio
    {
        namespace sqwave
        {
            static const uint16_t squarewave_program_instructions[] = {
                //     .wrap_target
                0xe081,    //  0: set    pindirs, 1
                0xe101,    //  1: set    pins, 1                [1]
                0xe000,    //  2: set    pins, 0
                0x0001,    //  3: jmp    1
                           //     .wrap
            };

#if !PICO_NO_HARDWARE
            static const struct pio_program squarewave_program = {
                .instructions = squarewave_program_instructions,
                .length       = 4,
                .origin       = -1,
            };

#    define squarewave_wrap_target 0
#    define squarewave_wrap 3
            static inline pio_sm_config squarewave_program_get_default_config(uint offset)
            {
                pio_sm_config c = pio_get_default_sm_config();
                sm_config_set_wrap(&c, offset + squarewave_wrap_target, offset + squarewave_wrap);
                return c;
            }
#    undef squarewave_wrap_target
#    undef squarewave_wrap
#endif
        }    // namespace sqwave
    }        // namespace pio
}    // namespace Udon
