# Gyro

## ジャイロセンサスレーブと通信し、旋回角を取得するクラス

このクラスでは、ジャイロセンサ 1 つに対して `Gyro` クラスのインスタンスを複数作成することができます。

よって、あるインスタンスの `clear` 関数を呼んだ場合でも、他のインスタンスの旋回角がクリアされる心配がありません。

# Usage

## API

-   `static Gyro::begin(uint32_t baudrate = 115200)`

    通信を開始する

    `setup` 関数で呼ぶ

-   `static Gyro::update()`

    値更新

    `loop` 関数で呼ぶ

-   `Gyro::clear()`

    旋回角を 0 にする

-   `Gyro::yaw()`

    旋回角を取得する

## シリアル切り替え

マスターによってシリアルインスタンスを切り替える場合、

`GYRO_SERIAL` の定義を変更することで切り替えられます

```cpp
//#define GYRO_SERIAL Serial  // arduino
#define GYRO_SERIAL Serial1  // teensy
```

## Example

```cpp
#include "Gyro.h"

Gyro positive;

void setup() {
	Serial.begin(115200);
	Gyro::begin(115200);
}

void loop() {
	Gyro::update();

	Serial.print(positive.yaw()); Serial.print('\n');

	delay(10);
}
```

複数インスタンス

```cpp
#include "Gyro.h"

Gyro positive;
Gyro turn;
Gyro hoge;

void setup() {
	Serial.begin(115200);
	Gyro::begin(115200);
}

void loop() {
	Gyro::update();

	if (millis() > 1000)  /// 1 秒後に positive.yaw() が 0 に
		positive.clear();

	if (millis() > 2000)  /// 2 秒後に turn.yaw() が 0 に
		turn.clear();

	Serial.print(positive.yaw()); Serial.print('\t');
	Serial.print(turn.yaw()); Serial.print('\t');
	Serial.print(hoge.yaw()); Serial.print('\n');

	delay(10);
}
```

また旋回角は static メンバによって管理されているので以下のようにもできます

```cpp
#include "Gyro.h"

void setup() {
	Serial.begin(115200);
	Gyro::begin(115200);
}

void loop() {
	Gyro::update();

	Serial.print(Gyro{}.yaw()); Serial.print('\n');

	delay(10);
}
```
