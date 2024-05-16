//
//    ロボマスモーター電流制御
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Udon.hpp>

// CAN通信バス
static Udon::CanBusTeensy<CAN2> bus;

// ロボマスモーター (C620ドライバ経由 モーターID: 2)
static Udon::RoboMasterC620 motor{ bus, 2 };

void setup()
{
    // CAN通信を開始
    bus.begin();
}

void loop()
{
    // CAN通信を更新
    bus.update();

    // 動作電流値を設定
    motor.setCurrent(10000);
}
