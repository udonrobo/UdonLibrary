#include <Udon.hpp>

// CAN通信バス
Udon::CanBusTeensy<CAN2> bus;

// ロボマスモーター (C620ドライバ経由 モータID: 2)
Udon::RoboMasterC620 motor{ bus, 2 };

void setup() {
    // CAN通信を開始
    bus.begin();
}

void loop() {

    // CAN通信を更新
    bus.update();

    // 設定動作電流値を設定
    motor.setCurrent(10000);

    // 回転角度を取得
    const auto angle = motor.getAngle();
    Serial.println(angle);

    // 回転速度を取得
    const auto rpm = motor.getVelocity();
    Serial.println(rpm);

    // トルク電流を取得
    const auto torqueCurrent = motor.getTorqueCurrent();
    Serial.println(torqueCurrent);

    // モーターの温度を取得
    const auto temp = motor.getTemperature();
    Serial.println(temp);

}
