# Gyro

## ジャイロセンサスレーブと通信し、旋回角を取得するクラス

このクラスでは、ジャイロセンサ 1 つに対して `Gyro` クラスのインスタンスを複数作成することができます。

よって、あるインスタンスの `clear` 関数を呼んだ場合でも、他のインスタンスの旋回角がクリアされる心配がありません。

# Usage

## Method

-   `static Gyro::init()`

    通信を開始する

    `setup` 関数で呼ぶ

-   `static Gyro::update()`

    値更新

    `loop` 関数で呼ぶ

-   `Gyro::clear()`

    旋回角を 0 にする

-   `Gyro::yaw()`

    旋回角を取得する

## シリアルポートの切り替え

マスターによってシリアルクラスのインスタンスを切り替える必要があります

`SERIAL_INSTANCE` の定義を変更することで切り替えられます

```cpp
//#define SERIAL_INSTANCE Serial  // Arduino
#define SERIAL_INSTANCE Serial1  // teensy
```

## Example

```cpp
#include "Gyro.h"

Gyro gyro;

void setup() {
	Serial.begin(115200);
	Gyro::init();
}

void loop() {
	Gyro::update();

	Serial.println(gyro.yaw());

	delay(10);
}
```
