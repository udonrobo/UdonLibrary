//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Driver/DipSwitch.hpp>

inline void test()
{
    (void)Udon::DecodeDipSwitch({ 2, 3, 4, 5 });
}
