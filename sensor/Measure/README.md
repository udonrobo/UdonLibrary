# Measure

## Description

計測輪の情報をもとに絶対座標に変換するクラスです

## Data

-   `@date` 2022/09/22
-   `@author` 大河 祐介

# Usage

## API

-   コンストラクタ

    -   `Measure(EncoderBoardTeensy& enc, portX, portY)`

        EncoderBoardTeensy クラスから情報を読み取るための設定をする

        `@param enc` EncoderBoardTeensy クラスのインスタンス参照

        `@param portX` X 軸のエンコーダーの入力ポート番号

        `@param portY` Y 軸のエンコーダーの入力ポート番号

-   値更新

    -   `void update()`

-   値取得、消去

    -   `double x()`

    -   `double y()`

    -   `void clear(offset = 0)`

        `@param offset` スタート時のオフセット

-   デバッグ出力

    -   `show(end = {})`

        `@param end` 最後に出力される文字

## Example

```cpp
#include "Measure.h"

EncoderBoardTeensy enc(0);
Measure measure(enc, 1, 2);

void setup() {
	Serial.begin(115200);
	Gyro::begin(115200);
	enc.init();
}

void loop() {
	Gyro::update();
	measure.update();

	Serial.print(measure.x()); Serial.print('\t');
	Serial.print(measure.y()); Serial.print('\n');

	delay(10);
}
```
