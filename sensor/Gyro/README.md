# Gyro

## Description

ジャイロセンサスレーブと通信し、旋回角を取得するクラス

ジャイロセンサ 1 つに対して `Gyro` クラスのインスタンスを複数作成することができます

そのため、角度の使用用途に応じてインスタンスを作ることができます(旋回補正用、ポジティブ制御用等)

## Data

-   `@date` 2022/09/22
-   `@author` 大河 祐介

# Usage

## API

-   通信開始

    -   `static void begin(baudrate = 115200)`

-   値更新

    -   `static void update(offsetDeg = 0)`

        `@param offset` 旋回した状態でスタートする時などに使います

-   値取得、消去

    -   `void clear()`

    -   `double yaw()`

-   デバッグ出力

    -   `show(end = {})`

        `@param end` 最後に出力される文字

## Communication

シリアルインスタンスを切り替える場合、
`GYRO_SERIAL` の定義を変更することで切り替えられます

```cpp
// #define GYRO_SERIAL Serial
#define GYRO_SERIAL Serial1
// #define GYRO_SERIAL Serial2
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

また旋回角は static メンバによって管理されているので、クリアする必要がない場合以下のようにもできます

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
