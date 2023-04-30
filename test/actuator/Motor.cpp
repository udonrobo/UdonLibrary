#include <udon/actuator/Motor.hpp>

inline void test()
{
	{
		udon::Motor2 motor{0, 0};
		motor.move(0);
		motor.show();
	}

	{
		udon::Motor3 motor{0, 0, 0};
		motor.move(0);
		motor.show();
	}
}