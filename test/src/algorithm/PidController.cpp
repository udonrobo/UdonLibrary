#include <udon/algorithm/PidController.hpp>

inline void test()
{
    udon::PidController pid{ 0.0, 0.0, 0.0, 1000 };
    pid.setParamPro(0.0);
    pid.setParamInt(0.0);
    pid.setParamDif(0.0);
    pid.getProPower();
    pid.getIntPower();
    pid.getDifPower();
    pid.clearPower();
    pid.getPower();
    pid.update(10, 20);
}