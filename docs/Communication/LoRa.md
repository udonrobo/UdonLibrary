# LoRa

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/8336c610-8d65-4c18-a0bf-d64c82b55e08" width="250px" align="right"/>

920MHz 帯無線モジュール

## 使用部品

- [E220](https://akizukidenshi.com/catalog/g/g117616/)
- [アンテナ](https://akizukidenshi.com/catalog/g/g117618/)

## E220 LoRa モジュール

- ペアリングが必要ありません。
- 秋月電子で購入でき安価 (2000 円程度)
- モジュールとは UART で通信し、かつ受信完了を AUX ピンから確認できるため、UART でよく起こるバイト列のズレが起こりません。
- バイト列を直接送信できます (文字列にする必要がない)
- 受信間隔長め (8byte で 100ms 程度)

## 個別インクルード

```cpp
#include <Udon/Com/LoRa.hpp>
```

## 概要

双方向通信には対応していません。uint64_t 型のオブジェクトを送信する例です。

### スケッチ例 / 送信側

```cpp
#include <Udon.hpp>

static Udon::E220Writer<uint64_t> lora({
    .serial  = Serial1,
    .m0      = 2,
    .m1      = 3,
    .aux     = 4,
    .channel = 0,
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

### スケッチ例 / 受信側

```cpp
#include <Udon.hpp>

static Udon::E220Reader<uint64_t> lora({
    .serial  = Serial1,
    .m0      = 2,
    .m1      = 3,
    .aux     = 4,
    .channel = 0,
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
    else
    {
        Serial.println("receive timeout!");
    }
    delay(10);
}
```

## チャンネルの設定

`begin` 関数の引数でもチェンネルを指定できます。この時コンストラクタでチャンネルを指定する必要はありません。

```cpp
#include <Udon.hpp>

static Udon::E220Writer<uint64_t> lora({
    .serial  = Serial1,
    .m0      = 2,
    .m1      = 3,
    .aux     = 4,
    // チャンネル番号を省略
});

void setup()
{
    const int channel = 10;
    lora.begin(channel);
}
```

## その他設定値の変更

設定用構造体は以下のように定義されています。

デフォルトはユニキャスト通信(1 対 1)です。送信側のアドレスを `0xffff` にすることでブロードキャスト通信(1 対多)になります。この時受信側アドレスは無視されます。

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
    /// @note 0x0000-0xFFFF
    /// @note 相手と合わせる
    uint16_t address = 0x0102;
};
```

コンストラクタで設定した値は `getConfigReference()` メンバ関数を使用して変更可能です。変更は `begin()` メンバ関数を呼び出す前に行う必要があります。

```cpp
#include <Udon.hpp>

static Udon::E220Writer<uint64_t> lora({
    .serial = Serial1,
    .m0 = 2,
    .m1 = 3,
    .aux = 4,
    // チャンネル番号を省略
});

void setup()
{
    lora.getConfigReference().channel = 2;
    lora.begin();
}
```

## 受信強度

`getRssi()` メンバ関数を使用して電波強度を示す RSSI 値を取得できます。0 に近いほど電波強度が高いことを示します。値は通信できている場合のみ有効です。

```cpp
void loop()
{
    if (const auto m = lora.getMessage())
    {
        const int rssi = lora.getRssi();
    }
}
```

[metageek 社による指標](https://www.metageek.com/training/resources/understanding-rssi/) では RSSI 値と品質の対応は次のようになっています。

| RSSI [dBm] | 品質       |
| ---------- | ---------- |
| -30        | 非常に強い |
| -67        | とても強い |
| -70        | 強い       |
| -80        | 弱い       |
| -90        | 使えない   |

## 周波数

チャンネルを変更することで周波数帯の調整ができます。チャンネルから周波数は以下式で求められます。

$$
周波数 = 920.8MHz + チャンネル番号×200kHz
$$
