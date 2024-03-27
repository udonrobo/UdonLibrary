# CAN

CAN 通信クラスは、通信バスクラス、送受信ノードクラスから構成されています。

- [通信バスクラス](#通信バスクラス)
  - [Teensy](#teensy)
  - [Raspberry Pi Pico](#raspberry-pi-pico)
  - [インターフェース](#インターフェース)
- [送信クラス](#送信クラス)
- [受信クラス](#受信クラス)
- [デバッグ](#デバッグ)
- [クラスの組み合わせ色々](#クラスの組み合わせ色々)

<details>
<summary> CAN 通信について </summary>

複数のデバイス間で通信を行う際の通信方式

### 用語

`バス` 通信線

`ノード` バスに接続されているマイコン等 (マイコン内にも複数作成可能)

`ノードID` 送信ノードの識別をするための固有値

### 特徴

`非同期通信`

`差動通信` ノイズの影響を受けにくい

`バス型通信` 複数のデバイスが同じバスを共有して通信

`マルチマスター方式` 各ノードが好きなタイミングでデータを送受信可能

受信する際は、流れているデータに含まれる ノード ID を見て受信 (送信者は関知しない)

> 詳しくは VECTOR 社の PDF がわかりやすかったので、そちらをご覧ください。[はじめての CAN / CAN FD](https://cdn.vector.com/cms/content/know-how/VJ/PDF/For_Beginners_CAN_CANFD.pdf)

### 通信イメージ

```mermaid
flowchart LR
    メインノード --CAN--> ロガーノード
    メインノード <--CAN-->
    コントローラーノード --CAN--> ロガーノード
    メインノード --CAN--> 1[モーターノード]
    メインノード --CAN--> 2[モーターノード]
    2[モーターノード] --CAN--> ロガーノード
```

</details>

## 個別インクルード

```cpp
#include <Udon/Com/Can.hpp>
```

## 通信バスクラス

送受信処理、通信が行えているかどうかのチェックを行います。使用するマイコンや CAN コントローラーによってバスクラスは異なります。

> CAN の通信プロトコルは `CAN2.0B` を使用します。よって ID は `0x000` ~ `0x7FF` の範囲で使用できます。
>
> CAN2.0B は 一度に 8byte のデータしか送信できません。よって 8byte より多いデータは分割し送受信します。この時、1byte 目にはインデックス番号が付与されます。
>
> 8byte 以下のデータはインデックスを付与せず送受信するため、他の CAN デバイスとの通信にも使用できます。

### Teensy

内臓 CAN コントローラーを使用し CAN 通信を行います。受信ノードが 8 個以内の場合、受信フィルタの設定を行います。

```mermaid
flowchart LR

  subgraph 基板
    Teensy --CAN TX/RX--> CANトランシーバー
  end

CANトランシーバー --CAN H/L--> CANバス
```

```cpp
static Udon::CanBusTeensy<CAN1> bus;
```

<details>
<summary> 詳細な設定について </summary>

`Udon::CanBusTeensy::Config` 構造体を用いて詳細な設定が可能です。構造体は次のように定義されています。

```cpp
struct Config
{
    uint32_t transmitInterval = 5;            // 送信間隔 [ms]
    uint32_t transmitTimeout  = 100;          // 送信タイムアウト時間 [ms]
    uint32_t receiveTimeout   = 100;          // 受信タイムアウト時間 [ms]
    uint32_t canBaudrate      = 1'000'000;    // CAN通信速度 [bps]
};
```

```cpp
static Udon::CanBusTeensy<CAN1> bus({
    .transmitInterval = 5,
    .transmitTimeout  = 100,
    .receiveTimeout   = 100,
    .canBaudrate      = 1'000'000,
});
```

</details>

### Raspberry Pi Pico

外部 CAN コントローラーを使用し CAN 通信を行います。コントローラーとは SPI で通信します。受信ノードが 6 個以内の場合、受信フィルタの設定を行います。

```mermaid
flowchart LR

  subgraph 基板
  RaspberryPiPico --SPI--> CANコントローラー/MCP2515 --CAN TX/RX--> CANトランシーバー
  end

  CANトランシーバー --CAN H/L--> CANバス
```

```cpp
static Udon::CanBusSpi bus({
    .interrupt = 2
});
```

<details>
<summary> 詳細な設定について </summary>

ピン設定等は設定値を格納するための構造体 `Udon::CanBusSpi::Config` を用いて設定します。これらの構造体は次のように定義されています。

```cpp
struct Config
{
    // SPI 関連
    spi_inst_t* channel = spi_default;                 // SPI チャンネル (spi0, spi1)
    uint8_t     cs      = PICO_DEFAULT_SPI_CSN_PIN;    // チップセレクトピン
    uint8_t     interrupt;                             // 受信割り込みピン
    uint8_t     mosi = PICO_DEFAULT_SPI_TX_PIN;        // MOSIピン (TX)
    uint8_t     miso = PICO_DEFAULT_SPI_RX_PIN;        // MISOピン (RX)
    uint8_t     sck  = PICO_DEFAULT_SPI_SCK_PIN;       // クロックピン
    uint32_t    spiClock = 1'000'000;    // SPIクロック周波数 [Hz]

    // CAN 関連
    uint32_t  transmitInterval = 5;               // 送信間隔 [ms]
    uint32_t  transmitTimeout  = 100;             // 送信タイムアウト時間 [ms]
    uint32_t  receiveTimeout   = 100;             // 受信タイムアウト時間 [ms]
    CAN_SPEED canBaudrate      = CAN_1000KBPS;    // CAN通信速度
    CAN_CLOCK mcpClock         = MCP_16MHZ;       // トランシーバー動作クロック周波数 [Hz]
};
```

C99 対応コンパイラでは、構造体の初期化時に、メンバ変数名を指定することができます。メンバ変数を指定することで、インスタンス化部分を見たときに、引数の値が何の意味を持つかパット見で分かります。

```cpp
static Udon::CanBusSpi bus{ spi0, 0, 1, 2, 3, 4 };  // 各値の意味がコンストラクタの実装を見ないと分からない
↓
static Udon::CanBusSpi bus({
    .channel   = spi0,
    .cs        = 0,
    .interrupt = 1,
    .mosi      = 2,
    .miso      = 3,
    .sck       = 4
});
```

</details>

### インターフェース

すべてのバスクラスを一様に扱えるようにするためのインターフェースクラス `Udon::ICanBus` クラスがあります。送受信クラスのコンストラクタの引数はこのインターフェースクラスになっており、どの CAN バスを受け取ることができます。

## 送信クラス

`Udon::CanWriter<T>`

`T` に指定された型のオブジェクトをバスへ送信します。

```cpp
static Udon::CanBusTeensy<CAN1> bus;
static Udon::CanWriter<Udon::Vec2> writer{ bus, 0x010 };  // Udon::Vec2 をノードID 0x010 として送信

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    Udon::Vec2 vector{ 100.0, 200.0 };    // 送信するオブジェクトをインスタンス化
    writer.setMessage(vector);            // オブジェクトを登録

    delay(10);
}
```

## 受信クラス

`Udon::CanReader<T>`

`T` に指定された型のオブジェクトをバスから取得します。送信クラスの `T` と同じ型である必要があります。

```cpp
static Udon::CanBusTeensy<CAN1> bus;
static Udon::CanReader<Udon::Vec2> reader{ bus, 0x010 };  // ノードID 0x010 から Udon::Vec2を受信

void setup()
{
    bus.begin();
}
void loop()
{
    bus.update();

    if (const Udon::Optional<Udon::Vec2> vector = reader.getMessage())  // データ取得
    {
        // 受信成功
        vector->show();
        Serial.println();
    }
    else
    {
        // 受信失敗(データ破損検出、通信タイムアウト等)
        Serial.println("receive failed!!");
    }

    delay(10);
}
```

> `getMessage` は正常にオブジェクトが受信できたかどうか判定できるように `Udon::Optional<T>` を返します。通信エラー時は `Udon::nullopt` が返されます。
> `Udon::Optional` は `operator bool` を持っており、Optional が値を保持するとき true を返します。よって if 文で正常に受信できたかどうかで分岐できます。
>
> `Udon::Optional<T>::operator->` で保持しているオブジェクトのメンバへアクセスでき、`Udon::Optional<T>::operator*` で optional が持っているオブジェクトの参照を取得できます。

## デバッグ

全 CAN 通信クラスは `show()` メンバ関数を持っており、通信の状態をシリアルモニターへ送信します。

```cpp
bus.show();     // バスに参加している送受信ノードの列挙、送受信データ(バイト列)を表示
reader.show();  // 受信データを表示
writer.show();  // 送信データを表示
```

## エラー原因

通信が行われない -> Udon::PioClockBegin() の呼び忘れ

## クラスの組み合わせ色々

一つのバスへ複数送受信ノードが参加する(よくある)

```cpp
static Udon::CanBusTeensy<CAN1> bus;
static Udon::CanWriter<Udon::Vec2> writer1{ bus, 0x011 };
static Udon::CanWriter<Udon::Vec2> writer2{ bus, 0x012 };
static Udon::CanReader<Udon::Vec2> reader1{ bus, 0x013 };
static Udon::CanReader<Udon::Vec2> reader2{ bus, 0x014 };
```

二つのバスへ受信ノードが参加する(バスの負荷分散目的)

```cpp
static Udon::CanBusTeensy<CAN1> bus1;
static Udon::CanWriter<Udon::Vec2> writer1{ bus1, 0x011 };
static Udon::CanReader<Udon::Vec2> reader1{ bus1, 0x012 };

static Udon::CanBusTeensy<CAN2> bus2;
static Udon::CanWriter<Udon::Vec2> writer2{ bus2, 0x011 };  // バスが異なるのでID重複してもOK
static Udon::CanReader<Udon::Vec2> reader2{ bus2, 0x012 };
```

異なる種類のバスへ参加する(激レア)

```cpp
static Udon::CanBusTeensy<CAN1> bus1;
static Udon::CanWriter<Udon::Vec2> writer1{ bus1, 0x011 };
static Udon::CanReader<Udon::Vec2> reader1{ bus1, 0x013 };

static Udon::CanBusSpi bus2({ ... });
static Udon::CanWriter<Udon::Vec2> writer2{ bus2, 0x012 };
static Udon::CanReader<Udon::Vec2> reader2{ bus2, 0x014 };
```
