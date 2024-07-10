# CAN 通信

CAN 通信は自動車内部の通信等工業系で主に用いられる通信手法です。差動通信であるためノイズ耐性が高く、またバス型方式をとっているため容易に配線が可能です。

送信者は送信ノードと呼ばれ、各送信ノードは自由なタイミングで送信を行えます。他の送信ノードが送信中の場合、調停を行い送信タイミングを自動的に調整します。受信者は受信ノードと呼ばれ、送信者が持つ識別子を識別し受信を行います。

CAN 通信クラスは、通信バスクラス、送受信ノードクラスから構成され、組み合わせて使用します。

- [通信バスクラス](#通信バスクラス)
- [送信ノードクラス](#送信ノードクラス)
- [受信ノードクラス](#受信ノードクラス)
- [デバッグ](#デバッグ)
- [クラスの組み合わせ色々](#クラスの組み合わせ色々)

## 個別インクルード

```cpp
#include <Udon/Com/Can.hpp>
```

## 通信バスクラス

送受信処理、通信が行えているかどうかのチェックを行います。使用するマイコンや CAN コントローラーによって適切なバスクラスを選択します。

CAN2.0A プロトコルで通信を行います。そのため ID は 0x000~0x7FF の範囲で使用できます。

CAN2.0A は 一度に 8 バイトのデータしか送信できません。8 バイトより長いデータは分割して送受信します。この時、1 バイト目にインデックス番号が付与されます。8 バイト以下のデータはインデックスを付与せず送受信するため、他の CAN デバイス (市販モーター等) との通信にも使用できます。詳しくは [バイト列を直接送受信](#バイト列を直接送受信) を参照ください。

### ■ Teensy

内臓 CAN コントローラーを使用し CAN 通信を行います。受信ノードが 8 個以内の場合、受信フィルタの設定を行います。

```mermaid
flowchart LR
  subgraph 基板
    Teensy --CAN TX/RX--> CANトランシーバー
  end
  CANトランシーバー --CAN H/L--> CANバス
```

クラスのテンプレートパラメーターに使用するCANポートを指定します。

```cpp
static Udon::CanBusTeensy<CAN1> bus;
```

<details>
<summary> 詳細な設定 </summary>

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

<details>
<summary> 回路 </summary>

Teensy4.0 + MCP2562 の例を示します。

- Teensy4.0 の CAN1 ポートと接続しています。
- 同じ名前の配線は内部的に接続されています。
- 終端抵抗の有無を切り替えられるようにする必要があります。
- MCP2562 の VIO 端子は、トランシーバー側 IO 端子の基準電源として機能します。マイコン側の IO 端子電圧（0〜3.3V）に一致するように、この端子は 3.3V に接続されています。

![image](https://github.com/udonrobo/UdonLibrary/assets/91818705/a4ec4c96-004a-41af-bd52-19bd71f41370)

</details>

### ■ Raspberry Pi Pico

外部 CAN コントローラー (MCP2515 or MCP25625) を使用し CAN 通信を行います。受信ノードが 6 個以内の場合、受信フィルタの設定を行います。受信フィルタが有効の場合、登録していない ID の受信を関知しないため、不要な処理の削減につながります。

```mermaid
flowchart LR
  subgraph 基板
    RaspberryPiPico --SPI--> CANコントローラー --CAN TX/RX--> CANトランシーバー
  end
  CANトランシーバー --CAN H/L--> CANバス
```

```cpp
static Udon::CanBusSpi bus;
```

<details>
<summary> 詳細な設定 </summary>

ピン設定等は設定値を格納するための構造体 `Udon::CanBusSpi::Config` を用いて設定します。これらの構造体は次のように定義されています。

```cpp
struct Config
{
    // SPI 関連
    spi_inst_t* channel     = spi_default;                // SPI チャンネル (spi0, spi1)
    uint8_t     cs          = PICO_DEFAULT_SPI_CSN_PIN;   // チップセレクトピン
    uint8_t     mosi        = PICO_DEFAULT_SPI_TX_PIN;    // MOSIピン (TX)
    uint8_t     miso        = PICO_DEFAULT_SPI_RX_PIN;    // MISOピン (RX)
    uint8_t     sck         = PICO_DEFAULT_SPI_SCK_PIN;   // クロックピン
    uint8_t     interrupt   = 20;                         // 受信割り込みピン
    uint32_t    spiClock    = 1'000'000;                  // SPIクロック周波数 [Hz]

    // CAN 関連
    uint32_t  transmitInterval  = 5;               // 送信間隔 [ms]
    uint32_t  transmitTimeout   = 100;             // 送信タイムアウト時間 [ms]
    uint32_t  receiveTimeout    = 100;             // 受信タイムアウト時間 [ms]
    CAN_SPEED canBaudrate       = CAN_1000KBPS;    // CAN通信速度
    CAN_CLOCK mcpClock          = MCP_16MHZ;       // トランシーバー動作クロック周波数 [Hz]
};
```

</details>

## 送信ノードクラス

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

## 受信ノードクラス

`Udon::CanReader<T>`

`T` に指定された型のオブジェクトをバスから取得します。送信ノードの `T` と同じ型である必要があります。

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
        Serial.print(vector->x); Serial.print('\t');
        Serial.print(vector->y); Serial.print('\n');
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
> `Udon::Optional` は値と値が有効であるかを持つクラスで、 `operator bool` によって値を保持するか取得できます。if 文で正常に受信できたかどうかで分岐できます。
>
> `Udon::Optional<T>::operator->` で保持しているオブジェクトのメンバへアクセスでき、`Udon::Optional<T>::operator*` で optional が持っているオブジェクトの参照を取得できます。

## バイト列を直接送受信

ロボマスモーター等の CAN 通信を用いる市販のモーターをドライブするには、バイト列で直接やり取りする必要があります。この場合、チェックサムを付与する `CanReader` `CanWriter` クラスは使用できません。バイト列を直接やり取りするには送受信ノードを `createTx` `createRx` 関数を用いて作成し、作成したノードに対しデータの書き込み、読み出しを行います。

### ■ 送信ノード

<details>
<summary> 送信ノードの構造 </summary>

```cpp
struct CanTxNode
{
    const uint32_t id;            // メッセージ ID (バスが書き込み、ユーザーが読み取り)

    std::vector<uint8_t> data;    // 送信データ (ユーザーが書き込み、バスが読み取り)

    uint32_t transmitMs;          // 最終通信時刻 (バスが書き込み、ユーザーが読み取り)
};
```

</details>

<details>
<summary> 基本的な例 </summary>

```cpp
static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanTxNode* txNode = bus.createTx(0x000 /*id*/, 8 /*length*/);

void setup()
{
    bus.begin();
}

void loop()
{
    txNode->data[0] = 0x11;  // データを登録
    txNode->data[1] = 0x22;
    txNode->data[2] = 0x11;
    txNode->data[3] = 0x22;
    txNode->data[4] = 0x11;
    txNode->data[5] = 0x22;
    txNode->data[6] = 0x11;
    txNode->data[7] = 0x22;
    bus.update();
}
```

</details>

<details>
<summary> ラッパークラスを作成する例 </summary>

```cpp
#include <Udon.hpp>

class MyCanWriter
{
    Udon::CanTxNode* txNode;

public:
    MyCanWriter(Udon::ICanBus& bus, uint8_t id, size_t length)
        : txNode(bus.createTx(id, length))
    {
    }

    void update()
    {
        txNode->data[0] = 0x11;  // データを登録
        txNode->data[1] = 0x22;
    }
};

static Udon::CanBusTeensy<CAN1> bus;

static MyCanWriter writer{ bus, 0x000, 10 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();
    writer.update();
}
```

</details>

### ■ 受信ノード

<details>
<summary> 受信ノードの構造 </summary>

```cpp
struct CanRxNode
{
    const uint32_t id;           // メッセージID (バスが書き込み、ユーザーが読み取り)

    std::vector<uint8_t> data;   // 受信データ (バスが書き込み、ユーザーが読み取り)

    void (*onReceive)(void*);    // 受信時コールバック (ユーザーが書き込み)
    void* param;                 // コールバックパラメータ (ユーザーが書き込み)

    uint32_t transmitMs;         // 最終通信時刻 (バスが書き込み、ユーザーが読み取り)
};
```

</details>

<details>
<summary> 基本的な例 </summary>

```cpp
static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanRxNode* rxNode = bus.createRx(0x000 /*id*/, 8 /*length*/);

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    Serial.print(rxNode->data[0]); Serial.print('\t');
    Serial.print(rxNode->data[1]); Serial.print('\t');
    Serial.print(rxNode->data[2]); Serial.print('\t');
    Serial.print(rxNode->data[3]); Serial.print('\t');
    Serial.print(rxNode->data[4]); Serial.print('\t');
    Serial.print(rxNode->data[5]); Serial.print('\t');
    Serial.print(rxNode->data[6]); Serial.print('\t');
    Serial.print(rxNode->data[7]); Serial.print('\n');
}
```

</details>

<details>
<summary> コールバックを行う例 </summary>

受信ノードには、受信時に呼び出すコールバック関数を登録できます。8 バイトより長いバイト列は複数のフレームに分割されて送信されます。この時、コールバック関数が呼び出されるのは最終フレーム受信時です。

コールバック関数には `param` メンバを通じて任意の void ポインタを渡せます。以下の例では CanRxNode ポインタを渡しています。

```cpp
static Udon::CanBusTeensy<CAN1> bus;

static Udon::CanRxNode* rxNode = bus.createRx(0x000 /*id*/, 8 /*length*/);

// コールバック関数
void onReceive(void* param)
{
    auto node = static_cast<Udon::CanRxNode*>(param);   // 受信ノードを void ポインタから復元

    Serial.print(node->id); Serial.print('\t');

    Serial.print(node->data[0]); Serial.print('\t');
    Serial.print(node->data[1]); Serial.print('\t');
    Serial.print(node->data[2]); Serial.print('\t');
    Serial.print(node->data[3]); Serial.print('\t');
    Serial.print(node->data[4]); Serial.print('\t');
    Serial.print(node->data[5]); Serial.print('\t');
    Serial.print(node->data[6]); Serial.print('\t');
    Serial.print(node->data[7]); Serial.print('\n');
}

void setup()
{
    bus.begin();

    rxNode->onReceive = onReceive;   // 受信時コールバック関数を登録
    rxNode->param     = rxNode;      // コールバック関数の引数へ受信ノードを渡すよう指定
}

void loop()
{
    bus.update();
}
```

</details>

<details>
<summary> ラッパークラスを作成する例 </summary>

`param` メンバを通じて this ポインタを渡すことで、メンバへアクセスできます。`CanReader` クラスもこの機能を用いてメンバ関数のコールバックを行っています。

```cpp
#include <Udon.hpp>

class MyCanReader
{
    Udon::CanRxNode* rxNode;

    int16_t value;

public:
    MyCanReader(Udon::ICanBus& bus, uint8_t id, size_t length)
        : rxNode(bus.createRx(id, length))
    {
        rxNode->onReceive = onReceive;
        rxNode->param = this;   // this ポインタを登録
    }

    MyCanReader(const MyCanReader& other)
        : rxNode(other.rxNode)
    {
        rxNode.param = this;   // コピーされた場合、インスタンスが変わるので this ポインタを登録しなおす。
    }

    void update()
    {
        Serial.println(value);
    }

    static void onReceive(void* param)
    {
        auto self = static_cast<MyCanReader*>(param);  // this ポインタを復元
        self->value = (self->rxNode->data[0] << 8) | self->rxNode->data[1];
    }
};

static Udon::CanBusTeensy<CAN1> bus;

static MyCanReader reader{ bus, 0x000, 10 };

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();
    reader.update();
}
```

</details>

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
