#include <udon/actuator/ServoSpeed.hpp>

inline void test()
{
  udon::ServoSpeed servo(10000,ServoChoose::_20kg);
  servo.update();
  servo.setAngle(150,15);
  servo.stop();
  servo.getStatus();
  servo.getAngle();
  servo.speedValue();
}
