//
//    Raspberry Pi Pico 用ウォッチドックタイマー
//
//    Copyright (c) 2022 udonrobo
//

#include <Arduino.h>
#include <Udon/Utility/Assert.hpp>

__attribute__((unused)) static void test()
{
    Udon::Assert(1 + 1 == 2);
    Udon::Assert(1 + 1 == 2, "1 + 1 is 2 (^o^)");
    Udon::Assert(1 + 1 == 2, "1 + 1 is 2 (^o^)", Udon::AssertAction::Skip);
    Udon::Assert(1 + 1 == 2, "1 + 1 is 2 (^o^)", Udon::AssertAction::Abort);
}
