# ロボマスモーター

ロボマスモーターは CAN 通信経由で制御できるモーターです。

電流値を指定することで動作し、またエンコーダーを搭載しているため、回転数や回転角を取得できます。

## 個別インクルード

```cpp
#include <Udon/Driver/RoboMasterMotor.hpp>
```

## 概要

以下の表に示すように、使用するドライバによってクラスが異なります。また指定できる電流範囲も異なります。

使用される CAN ID は `0x200`, `0x1FF`, `0x200+モーターID` です。ドライバは 1ms 間隔で CAN フレームを送信するため、ドライバが接続されている CAN バスには、他の CAN デバイスを接続しないことをお勧めします。

|               |                                                    C610 ドライバ                                                     |                                                    C620 ドライバ                                                     |
| :-----------: | :------------------------------------------------------------------------------------------------------------------: | :------------------------------------------------------------------------------------------------------------------: |
|   イメージ    | <img width=400px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/cec39fc0-3fc2-4ece-acfa-8b98f2782ec3"> | <img width=400px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/251f923d-58ff-4da5-897c-06d0e66ab2b1"> |
| 対応モーター  |       <img width=400px src="https://github.com/user-attachments/assets/49981db5-3fd7-47c3-a524-add5e0d02d5d">        |       <img width=400px src="https://github.com/user-attachments/assets/f3b40e14-8541-4d72-a9d6-96c318d693ca">        |
|    クラス     |                                                `Udon::RoboMasterC610`                                                |                                                `Udon::RoboMasterC620`                                                |
| 電流範囲 (mA) |                                                   -10,000 ~ 10,000                                                   |                                                   -20,000 ~ 20,000                                                   |

## 電流値の指定

モーター1つに対して1つの `Udon::RoboMasterCxxx` インスタンスを作成し、`setCurrent` で電流値を指定することで動作します。電流範囲は先ほどの表のとおりです。範囲外の値を指定した場合、範囲内の値にクリッピングされます。

```cpp
#include <Udon.hpp>

static Udon::CanBusTeensy<CAN2> bus;

static Udon::RoboMasterC620 motor{ bus, 1 };   // ロボマスモーター (ID: 1)

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    // 動作電流値を設定 (mA)
    motor.setCurrent(5000);
}
```

指定可能な電流の範囲は `getCurrentRange` メンバ関数より取得できます。フィードバック制御の制御値を制限する場合などに有用です。

```cpp
const Udon::Range<int16_t> currentRange = motor.getCurrentRange();

int16_t min = currentRange.min;  // 指定できる最小電流値 [mA]
int16_t max = currentRange.max;  // 指定できる最大電流値 [mA]
```

## センサー値取得

回転角度、回転速度、トルク電流、モーター温度を取得可能です。位置制御や速度制御をする場合、フィードバック制御と組み合わせて使用してください。

> [!NOTE]
>
> ドライバは駆動電源で動作するため、駆動電源が切れるとドライバに保存されているエンコーダー角が消去される点に注意してください。

```cpp
void loop()
{
    // 回転角度(無限角対応) [rad]
    const double angle = motor.getAngle();

    // 回転角度(無限角非対応) [rad]
    const double rawAngle = motor.getRawAngle();

    // 回転速度 [rpm]
    const int16_t rpm = motor.getVelocity();

    // トルク電流 [mA]
    const int16_t torqueCurrent = motor.getTorqueCurrent();

    // モーターの温度 [℃]
    const uint8_t temp = motor.getTemperature();
}
```

> ロボマスモーターから直接得られる回転角度は一周ごとに 0 になります。そこで `getAngle` 関数は過去の回転角を累積し、現在の角度を求めます。そのような理由から駆動電源が切れた場合、この関数が返す回転角は無効な値となります。

> `getRawAngle` 関数は、ロボマスモーターから直接得られる回転角度を取得する関数です。一周ごとに 0 に戻ります。

## 複数のモーターを制御

1 つの CAN バスに対して最大 8 つまでモーターを接続可能です。

モーター ID はドライバについている SET ボタンを押した後、ID 番号回ボタンを押すことで設定できます。モーター ID は緑に点滅する LED の点滅回数で確認できます。

```cpp
#include <Udon.hpp>

static Udon::CanBusTeensy<CAN2> bus;

// C620
static Udon::RoboMasterC620 motor1{ bus, 1 };
static Udon::RoboMasterC620 motor2{ bus, 2 };

// C610
static Udon::RoboMasterC610 motor3{ bus, 3 };
static Udon::RoboMasterC610 motor4{ bus, 4 };
```

## 複数 CAN バスの場合

複数の CAN バスを使用する場合、CAN バスクラスのオブジェクトを複数作成し、それぞれのモーターに対して対応する CAN バスを指定してください。

例えば Teensy の場合次のようになります。[詳細](./../Communication/CAN.md#-Teensy)

```cpp
#include <Udon.hpp>

static Udon::CanBusTeensy<CAN1> bus1;
static Udon::CanBusTeensy<CAN2> bus2;

static Udon::RoboMasterC620 motor1{ bus1, 1 };  // CAN1 バスに接続されたモーター (モーターID: 1)
static Udon::RoboMasterC620 motor2{ bus2, 1 };  // CAN2 バスに接続されたモーター (モーターID: 1)
```

## Raspberry Pi Pico での使用例

CAN バスクラスに `Udon::CanBusSpi` を使用してください。[詳細](./../Communication/CAN.md#-raspberry-pi-pico)

```cpp
#include <Udon.hpp>

static Udon::CanBusSpi bus;

static Udon::RoboMasterC620 motor{ bus, 1 };
```

## スケッチ例 (電流制御)

```cpp
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
```

## スケッチ例 (速度フィードバック制御)

```cpp
//
//    Copyright (c) 2022-2024 udonrobo
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
    const double targetVelocity = 5000;

    // 速度フィードバック制御をして目標速度に制御する
    const double current = pid(motor.getVelocity(), targetVelocity);
    motor.setCurrent(current);

    loopCtrl.update();
}
```

## スケッチ例 (速度制御クラスの作成例)

実用的な例として、ロボマスモーターの速度をフィードバック制御するクラスを作成します。複数のモーターを制御する場合、クラスを作成することでコードの見通しを良くすることができます。

```cpp
//
//    Copyright (c) 2022-2024 udonrobo
//
//    MotorSpeedFeedback.hpp
//

#pragma once

#include <Udon.hpp>

class MotorSpeedFeedback
{
    Udon::RoboMasterC620 motor;
    Udon::PidController  pid;

public:
    MotorSpeedFeedback(Udon::RoboMasterC620&& motor, Udon::PidController&& pid)
        : motor{ std::move(motor) }
        , pid{ std::move(pid) }
    {
    }

    void move(const double targetVelocity)
    {
        const auto current = pid(motor.getVelocity(), targetVelocity);
        motor.setCurrent(current);
    }

    void stop()
    {
        pid.clearPower();
        motor.setCurrent(0);
    }
};
```

```cpp
//
//    Copyright (c) 2022-2024 udonrobo
//
//    Main.ino
//

#include "MotorSpeedFeedback.hpp"

static Udon::CanBusTeensy<CAN2> bus;

static Udon::LoopCycleController loopCtrl{ 1000 };

static MotorSpeedFeedback motor1 {
    Udon::RoboMasterC620{ bus, 1 },
    Udon::PidController{ 2, 0, 0.03, loopCtrl.cycleUs() }
};

static MotorSpeedFeedback motor2 {
    Udon::RoboMasterC620{ bus, 2 },
    Udon::PidController{ 2, 0, 0.03, loopCtrl.cycleUs() }
};

static MotorSpeedFeedback motor3 {
    Udon::RoboMasterC620{ bus, 3 },
    Udon::PidController{ 2, 0, 0.03, loopCtrl.cycleUs() }
};

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    motor1.move(5000);
    motor2.move(5000);
    motor3.move(5000);

    loopCtrl.update();
}
```
