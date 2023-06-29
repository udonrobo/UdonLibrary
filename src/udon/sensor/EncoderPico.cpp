#include <udon/sensor/EncoderPico.hpp>

#if defined(ARDUINO_ARCH_RP2040)

namespace udon
{
    EncoderPico::EncoderStatus* EncoderPico::statuses[EncoderPico::PICO_GPOI_PIN_COUNT]; 
}

#endif    // defined(ARDUINO_ARCH_RP2040)