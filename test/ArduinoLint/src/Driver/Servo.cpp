//
//    構文テスト
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Driver/Servo.hpp>

#ifdef ARDUINO_ARCH_RP2040

inline void test()
{
    Udon::Servo servo;
    servo.attach(1);
    servo.write(90);
    servo.writeMicroseconds(1500);
    servo.read();
    servo.readMicroseconds();
}

#endif