#pragma once

#include <Udon/Driver/Pio/QuadratureEncoder.pio.hpp>

namespace Udon
{
    class EncoderPicoPio
    {
        PIO pio;
        int sm;

        bool begin()
        {

            PIOProgram pioLocator(&Pio::Encoder::quadrature_encoder_program);

            int offset;
            if (not pioLocator.prepare(&pio, &sm, &offset))
            {
                return false;
            }

            uint offset = pio_add_program(enc_pio, &quadrature_encoder_program);

            quadrature_encoder_program_init(pio, sm, offset, pin, 0);
        }

        int32_t getCount() const
        {
            return Pio::Encoder::quadrature_encoder_get_count(pio, sm);
        }
    };

}    // namespace Udon