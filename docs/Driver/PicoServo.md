# サーボ

`Udon::Servo` クラスを用いて、サーボモータを制御できます。メンバ関数は Arduino の `Servo` クラスと同じです。

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
