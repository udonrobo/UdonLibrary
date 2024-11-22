//
//    構文テスト
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Driver/SegmentsLed.hpp>

__attribute__((unused)) static void test()
{
    Udon::SegmentsLed led{
        { 2, 3, 4, 5, 6, 7, 8 },
        { 10, 11, 12, 13 },
    };

    led.begin();
    led.setValue(1234);
    led.show();
    led.showNan();
}
