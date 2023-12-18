# ロータリーエンコーダー

## EncoderPico クラス

### 個別インクルード

```cpp
#include <Udon/Driver/EncoderPico.hpp>
```

### 詳細

Raspberry Pi Pico 用エンコーダー計測クラス

エンコーダーを外部割込み、もしくは PIO を使用してエンコーダーのカウント値を取得します。

デフォルトでは、PIO を使用します。PIO を使用できない場合、外部割込みを使用します。次の場合 PIO を使用できません。

- エンコーダーの A ピン、B ピンが隣り合っていない場合
- 各 PIO 内の 4 つのステートマシンに空きがない場合
- 各 PIO に別のプログラムが既にロードされている場合 (PIO に空きがない場合)

PIO を使用したくない場合 `begin` 関数へ true を渡すことで、外部割り込みモードになります。

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

### APIs

```cpp
class EncoderPico
{

    /// @brief コンストラクタ
    EncoderPico(uint8_t pinA, uint8_t pinB);

    /// @brief 計測開始
    void begin(bool alwaysUseInterrupt = false);

    /// @brief エンコーダーのカウント値を取得する
    int32_t read() const;

    /// @brief エンコーダーのカウント値を表示する
    void show() const;

};
```
