//
//    ロボマスモーター速度フィードバック制御
//
//    Copyright (c) 2022 udonrobo
//

#include <Udon.hpp>

// ループ周期制御 (PID制御器のサンプリング周期を設定するために使用)
static Udon::LoopCycleController loopCtrl{ 1000 };

// CAN通信バス
static Udon::CanBusTeensy<CAN2> bus;

// ロボマスモーター (C620ドライバ経由 モーターID: 2)
static Udon::RoboMasterC620 motor{ bus, 2 };

// 速度をフィードバック制御するPID制御器
static Udon::PidController pid{ 2, 0, 0.03, loopCtrl.cycleUs() };


void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    // 目標速度 (rpm)
    const double targetSpeed = 5000;

    // 速度フィードバック制御をして目標速度に制御する
    const double ctrlPower = pid(motor.getVelocity(), targetSpeed);
    motor.setCurrent(ctrlPower);

    loopCtrl.update();
}
