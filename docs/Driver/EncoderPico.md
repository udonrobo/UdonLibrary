# EncoderPico クラス

Raspberry Pi Pico 用エンコーダー計測クラス

PIO を使用してエンコーダーのカウント値を計測します。以下の場合計測ができません。

- エンコーダーの A ピン、B ピンが隣り合っていない場合
- 各 PIO 内の 4 つのステートマシンに空きがない場合
- 各 PIO に別のプログラムが既にロードされている場合 (命令メモリに空きがない場合)

> 計測開始できたかは begin 関数の戻り値から取得できます。

## 個別インクルード

```cpp
#include <Udon/Driver/EncoderPico.hpp>
```

## スケッチ例

```cpp
#include <Udon.hpp>

Udon::EncoderPico encoder{ 0, 1 };

void setup()
{
    encoder.begin();
}

void loop()
{
    Serial.println(encoder.read());
    delay(10);
}
```

`show()` でカウント値を表示できます。

```cpp
void loop()
{
    encoder.show();
    Serial.println();
    delay(10);
}
```
