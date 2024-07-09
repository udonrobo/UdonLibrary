//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Driver/BNO055.hpp>

inline void test()
{
    Udon::BNO055 bno{ Wire };
    bno.begin();
    bno.clear();
    bno.update();
    bno.getQuaternion();
    bno.getEuler();
    bno.show();
}
