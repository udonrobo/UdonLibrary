# Gyro

## ジャイロセンサスレーブと通信し、旋回角を取得するクラス

このクラスでは、ジャイロセンサ 1 つに対して `Gyro` クラスのインスタンスを複数作成することができます。

よって、あるインスタンスの `clear` 関数を呼んだ場合でも、他のインスタンスの旋回角がクリアされる心配がありません。

# Usage

## API

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

## シリアルインスタンスの切り替え

マスターによってシリアルインスタンスを切り替える必要があります

`SERIAL_INSTANCE` の定義を変更することで切り替えられます

```cpp
//#define SERIAL_INSTANCE Serial  // Arduino
#define SERIAL_INSTANCE Serial1  // teensy
```

## Example

```cpp
#include "Gyro.h"

Gyro positive;

void setup() {
	Serial.begin(115200);
	Gyro::begin();
}

void loop() {
	Gyro::update();

	Serial.print(positive.yaw()); Serial.print('\n');

	delay(10);
}
```

また旋回角は static メンバによって管理されているので以下のようにもできます

```cpp
#include "Gyro.h"

void setup() {
	Serial.begin(115200);
	Gyro::begin();
}

void loop() {
	Gyro::update();

	Serial.print(Gyro{}.yaw()); Serial.print('\n');

	delay(10);
}
```
