#include <Udon/Actuator/MotorDriver.hpp>

inline void test()
{
    {
        Udon::Motor2Driver motor{ 0, 0 };
        motor.move(0);
        motor.stop();
        motor.show();
    }

    {
        Udon::Motor3Driver motor{ 0, 0, 0 };
        motor.move(0);
        motor.stop();
        motor.show();
    }
}