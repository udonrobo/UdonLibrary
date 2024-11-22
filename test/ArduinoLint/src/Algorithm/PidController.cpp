//
//    構文テスト
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Algorithm/PidController.hpp>

__attribute__((unused)) static void test()
{
    Udon::PidController pid{ 0.0, 0.0, 0.0, 1000 };

    pid.update(0.0, 0.0);
    pid.getPower();
    pid.getPower(-250, 250);

    (void)pid(0.0, 0.0);
    (void)pid(0.0, 0.0, -250, 250);

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