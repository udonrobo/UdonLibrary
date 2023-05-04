#include <udon/actuator/ServoSpeed.hpp>

inline void test()
{
  udon::ServoSpeed servo(10000,udon::ServoChoose::_20kg,10);
  servo.update();
  servo.setAngle(150,15.3);
  servo.stop();
  servo.getStatus();
  servo.getAngle();
  servo.speedValue();
}
