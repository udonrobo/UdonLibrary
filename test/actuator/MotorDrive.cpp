#include <udon/actuator/MotorDrive.hpp>

inline void test()
{
	{
		udon::Motor2Drive motor{0, 0};
		motor.move(0);
		motor.show();
	}

	{
		udon::Motor3Drive motor{0, 0, 0};
		motor.move(0);
		motor.show();
	}
}