//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Udon/Driver/Motor.hpp>

static void test()
{
    {
        Udon::Motor2 motor{ 0, 0 };
        motor.move(0);
        motor.stop();
        motor.show();
    }

    {
        Udon::Motor3 motor{ 0, 0, 0 };
        motor.move(0);
        motor.stop();
        motor.show();
    }
}