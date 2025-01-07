//
//    構文テスト
//
//    Copyright (c) 2022 udonrobo
//

#include <Udon/Algorithm/ServoSpeed.hpp>

__attribute__((unused)) static void test()
{
    Udon::ServoSpeed servo(10000, Udon::ServoChoose::_20kg, 10);
    servo.update();
    servo.setAngle(150, 15.3);
    servo.stop();
    servo.getStatus();
    servo.getAngle();
    servo.speedValue();
}
