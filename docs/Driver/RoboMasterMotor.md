# ロボマスモーター

ロボマスモーターを C620 ドライバを使用してドライブする場合、`RoboMasterC620` クラスを使用します。

C610 ドライバ用クラスは今はありませんが、`RoboMasterC620` クラスでドライブできます。C610 ドライバは設定可能電流値が -10000~10000 [mA] である点に注意してください。

使用される CAN ID は `0x200+モーターID` です。ロボマスモータードライバは 1ms 間隔で CAN フレームを送信するため、ロボマスモーターが接続されている CAN バスには、他の CAN デバイスを接続しないことをお勧めします。

ドライバとは CAN で通信します。そのため CAN 通信クラスと組み合わせて使用します。`setCurrent` で電流値を指定することで動作します。

```cpp
#include <Udon.hpp>

Udon::CanBusTeensy<CAN1> bus;

// ロボマスモーター (C620ドライバ経由 モーターID: 1)
Udon::RoboMasterC620 motor{ bus, 1 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    // 動作電流値を設定 -20000~20000 (単位: mA)
    motor.setCurrent(10000);
}
```

ロボマスモーターはエンコーダーや電流センサ等が内蔵されているため、それらの値も取得できます。位置制御や速度制御をする場合、フィードバック制御と組み合わせて使用してください。

モータードライバは駆動電源で動作するため、非常停止ボタンを押すとドライバに保存されているエンコーダー角が消去される点に注意してください。

```cpp
void loop()
{
    // 回転角度 [rad]
    const double angle = motor.getAngle();

    // 回転速度 [rpm]
    const int16_t rpm = motor.getVelocity();

    // トルク電流 [mA]
    const int16_t torqueCurrent = motor.getTorqueCurrent();

    // モーターの温度 [℃]
    const uint8_t temp = motor.getTemperature();
}
```

モーター ID ごとに `RoboMasterC620` オブジェクトを生成することで、1 つの CAN バスに対して最大 8 つまでモーターを接続可能です。

```cpp
#include <Udon.hpp>

Udon::CanBusTeensy<CAN1> bus;

Udon::RoboMasterC620 motor1{ bus, 1 };
Udon::RoboMasterC620 motor2{ bus, 2 };
```
