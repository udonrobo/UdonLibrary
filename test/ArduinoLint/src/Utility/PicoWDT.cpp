//
//    Raspberry Pi Pico 用ウォッチドックタイマー
//
//    Copyright (c) 2022 udonrobo
//

#ifdef ARDUINO_ARCH_RP2040

#    include <Arduino.h>
#    include <Udon/Utility/PicoWDT.hpp>

__attribute__((unused)) static void test()
{
    Udon::PicoWDT wdt;
    wdt.update();
}

#endif
