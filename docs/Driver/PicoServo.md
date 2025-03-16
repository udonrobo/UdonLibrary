# サーボ

`Udon::Servo` クラスを用いて、サーボモータを制御できます。メンバ関数は Arduino の `Servo` クラスと同じです。

> [!NOTE]
> Arduino の Servo ライブラリとの違い
>
> Raspberry Pi Pico (RP2040) 用 Servo ライブラリは PIO を使って PWM を出力します。RP2040 に搭載されているステートマシンの数は 8 つなので、9 つ以上のサーボを制御することはできません。
>
> `Udon::Servo` クラスは PIO を使わずに PWM を出力するため、ステートマシン数の制限を受けません。

## 個別インクルード

```cpp
#include <Udon/Driver/Servo.hpp>
```

## スケッチ例

```cpp
#include <Udon.hpp>

static Udon::Servo servo;

void setup()
{
    servo.attach(1);
}

void loop()
{
    servo.write(90);
    delay(1000);
    servo.write(0);
    delay(1000);
}
```
