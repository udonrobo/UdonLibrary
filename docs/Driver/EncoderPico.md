# エンコーダー

`Udon::EncoderPico` クラス (Raspberry Pi Pico 用) を用いてエンコーダーのカウント値を取得します。

個別に動作するステートマシンを用いて高速ポーリングを行うため、カウント値の取りこぼしがほとんどありません。

Raspberry Pi Pico にはステートマシンが 8 個内蔵されているので、最大 8 個まで計測ができます。計測するには以下の条件を満たす必要があります。

- A, B ピンを接続する端子が隣り合っている
- 各 PIO 内のステートマシンに空きがある
- 各 PIO の命令メモリに空きがある(別のプログラムがロードされていない)

> 計測開始できたかは begin 関数の戻り値から取得できます。

## 個別インクルード

```cpp
#include <Udon/Driver/EncoderPico.hpp>
```

## スケッチ例

```cpp
#include <Udon.hpp>

static Udon::EncoderPico encoder{ 0, 1 };

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
