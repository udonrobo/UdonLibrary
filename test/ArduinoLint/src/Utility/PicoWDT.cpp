//
//    Raspberry Pi Pico 用ウォッチドックタイマー
//
//    Copyright (c) 2022-2024 udonrobo
//

#ifdef ARDUINO_ARCH_RP2040

#include <Arduino.h>
#include <Udon/Utility/PicoWDT.hpp>

static void test()
{
    Udon::PicoWDT wdt;
    wdt.update();
}

#endif
