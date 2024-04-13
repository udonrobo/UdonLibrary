#include <Arduino.h>
#include <Udon/Driver/EncoderPico.hpp>

#ifdef ARDUINO_ARCH_RP2040

inline void test()
{
    Udon::EncoderPico encoder{ 0, 0 };
    encoder.begin();
    encoder.read();
}

#endif