#include <Udon/Algorithm/ServoSpeed.hpp>

inline void test()
{
    Udon::ServoSpeed servo(10000, Udon::ServoChoose::_20kg, 10);
    servo.update();
    servo.setAngle(150, 15.3);
    servo.stop();
    servo.getStatus();
    servo.getAngle();
    servo.speedValue();
}

