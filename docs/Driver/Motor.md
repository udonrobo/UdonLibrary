# モーター

モータードライバを介してモーターを制御します。 `Motor2` `Motor3` クラスが含まれます。

## 個別インクルード

```cpp
#include <Udon/Driver/Motor.hpp>
```

## Motor2 クラス

信号線が 2 線 (dir + pwm) タイプのモータードライバの制御に使用します。

コンストラクタにモータードライバと接続しているピン番号を指定します。`move` 関数の引数に出力値を与えることでモーターを駆動できます。`-255 ~ 255` の範囲である必要があり、範囲外の値はクランプされます。

```cpp
#include <Udon.hpp>

static Udon::Motor2 motor(dirPin, pwmPin);

void setup()
{
    motor.begin();
}

void loop()
{
    int16_t power = -135;

    motor.move(power);

    // motor.stop();  // motor.move(0) と同じ意味
}
```

## Motor3 クラス

信号線が 3 線 (dirA + dirB + pwm) タイプのモータードライバの制御に使用します。

コンストラクタにモータードライバと接続しているピン番号を指定します。コンストラクタ以外の仕様は `Motor2` クラスと同じです。

```cpp
#include <Udon.hpp>

static Udon::Motor3 motor(dirPinA, dirPinB, pwmPin);

void setup()
{
    motor.begin();
}

void loop()
{
    int16_t power = -135;

    motor.move(power);

    // motor.stop();  // motor.move(0) と同じ意味
}
```

## デバッグ

`show` メンバ関数を使用することで、現在の出力値をシリアルモニターで確認できます。

```cpp
motor.show();
Serial.println();

// Udon::Showln(motor);
```

## その他

`Motorx` クラスは、出力値の急激な変化によるモーター、モータードライバへの負荷を防止するために、内部で移動平均クラスを使用しています。デフォルトのサンプリング回数は 50 です。これは目標となる出力値に 50 ループ目で到達することを意味します。

`Motorx` クラスは以下の様に `SmoothlyMotorx` のエイリアスとなっています。そのため `SmoothlyMotorx` を直接使用することで移動平均のサンプリング回数の調整ができます。

```cpp
using Motor2 = SmoothlyMotor2<50>;
```

```cpp
SmoothlyMotor2<100> motor{ 2, 3 };
```
