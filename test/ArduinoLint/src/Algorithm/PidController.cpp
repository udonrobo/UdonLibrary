#include <Udon/Algorithm/PidController.hpp>

inline void test()
{
    Udon::PidController pid{ 0.0, 0.0, 0.0, 1000 };
    
    pid.update(0.0, 0.0);
    pid.getPower();

    (void)pid(0.0, 0.0);

    pid.clearPower();

    pid.requestParamPro(0.0);
    pid.requestParamInt(0.0);
    pid.requestParamDif(0.0);
    pid.requestParam({ 0.0, 0.0, 0.0 });

    pid.setParamPro(0.0);
    pid.setParamInt(0.0);
    pid.setParamDif(0.0);
    pid.setParam({ 0.0, 0.0, 0.0 });

    (void)pid.getParamPro();
    (void)pid.getParamInt();
    (void)pid.getParamDif();
    (void)pid.getParam();

    (void)pid.getPowerPro();
    (void)pid.getPowerInt();
    (void)pid.getPowerDif();
}