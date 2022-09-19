# Measure

## 計測輪の情報をもとに絶対座標に変換するクラス

# Usage

## API

-   `Measure::Measure(EncoderBoardTeensy& enc, uint8_t portX, uint8_t portY)`

    EncoderBoardTeensy クラスから情報を読み取るための設定をする

    `enc` EncoderBoardTeensy クラスのインスタンス参照

    `portX` X 軸のエンコーダーの入力ポート

    `portY` Y 軸のエンコーダーの入力ポート

-   `Measure::update()`

    値更新

    `loop` 関数で呼ぶ

-   `Measure::clear(int16_t yaw = 0)`

    値を消去する

    `yaw` 角度のオフセット

-   `Measure::x()`

    X 座標を取得

-   `Measure::y()`

    Y 座標を取得

-   `Measure::show(char end = {})`

    シリアル出力

    `end` 出力の最後に挿入される(改行などを入れる)

## Example

```cpp
#include "Measure.h"

EncoderBoardTeensy enc(0);
Measure measure(enc, 1, 2);

void setup() {
	Serial.begin(115200);
	Gyro::begin();
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
