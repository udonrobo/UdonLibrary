# LoRa

- [E220](#e220)

## E220

<img width=300px src="../Assets/LoRaE220.jpg">

### スケッチ例 / 受信側

```cpp
#include <Udon.hpp>

Udon::E220Reader<uint64_t> lora({
    .serial = Serial1,
    .m0 = 2,
    .m1 = 3,
    .aux = 4,
    .channel = 0
});

void setup()
{
    lora.begin();
}

void loop()
{
    if (const auto message = lora.getMessage())
    {
        Serial.println(*message);
    }
    delay(1);
}
```

### スケッチ例 / 送信側

```cpp
#include <Udon.hpp>

Udon::E220Writer<uint64_t> lora({
    .serial = Serial1,
    .m0 = 2,
    .m1 = 3,
    .aux = 4,
    .channel = 0
});

void setup()
{
    lora.begin();
}

void loop()
{
    lora.setMessage(millis());
    delay(1);
}
```

### 詳細

`Udon::E220Reader`、`Udon::E220Writer` のテンプレート引数で通信するメッセージの型、コンストラクタでピン設定等を行います。設定用構造体は以下のように定義されています。

```cpp
struct Config
{
    HardwareSerial& serial;    // シリアルポート

    uint8_t m0;     // M0 ピン
    uint8_t m1;     // M1 ピン
    uint8_t aux;    // AUX ピン

    /// @brief チャンネル
    /// @note 0-12 (13-30も使用できますが通信速度が遅くなります)
    /// @note 相手と合わせる
    uint8_t channel = 0;

    /// @brief アドレス
    /// @note 0x0000-0xFFFD
    /// @note 相手と合わせる
    uint16_t address = 0x0000;
};
```

チャンネルを変更することで周波数帯の調整ができます。チャンネルから周波数は以下式で求められます。

$$
周波数 = 920.8MHz + チャンネル番号×200kHz
$$

コンストラクタで設定した値は `getConfigReference()` メンバ関数を使用して変更可能です。変更は `begin()` メンバ関数を呼び出す前に行う必要があります。

```cpp
void setup()
{
    lora.getConfigReference().channel = 2;
    lora.begin();
}
```
