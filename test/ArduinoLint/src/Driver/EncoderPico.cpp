//
//    構文テスト
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Driver/EncoderPico.hpp>

#ifdef ARDUINO_ARCH_RP2040

__attribute__((unused)) static void test()
{
    Udon::EncoderPico encoder{ 0, 0 };
    encoder.begin();
    encoder.read();
}

#endif