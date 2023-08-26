#include <Arduino.h>

#include "EncoderPico.hpp"

#if defined(ARDUINO_ARCH_RP2040)

namespace Udon
{
    EncoderPico::EncoderStatus* EncoderPico::statuses[EncoderPico::PICO_GPOI_PIN_COUNT];
}

#endif    // defined(ARDUINO_ARCH_RP2040)