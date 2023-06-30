#include <Udon/Actuator/MotorDrive.hpp>

inline void test()
{
    {
        Udon::Motor2Drive motor{ 0, 0 };
        motor.move(0);
        motor.show();
    }

    {
        Udon::Motor3Drive motor{ 0, 0, 0 };
        motor.move(0);
        motor.show();
    }
}