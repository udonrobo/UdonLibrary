# CAN 通信

## Description

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
flowchart
    メインノード --CAN--> ロガーノード
    メインノード <--CAN--> 
    コントローラーノード --CAN--> ロガーノード
    メインノード --CAN--> 1[モーターノード]
    メインノード --CAN--> 2[モーターノード]
    2[モーターノード] --CAN--> ロガーノード
```

## Usage

CAN 通信クラスは、通信バスクラス、送受信ノードクラスから構成されています。

### インクルード

```cpp
#include <Udon.hpp>
// or
#include <Udon/Com/Can.hpp>
```

### バスクラス

`Udon::CanBus---`

通信が行えているかどうかのチェック、送受信処理を行います。

- teensy 使用時

  ```cpp
  static Udon::CanBusTeensy<CAN1> bus;
  ```

- CAN コントローラ使用時 ( SPI 経由 )

  このクラスは SPI 通信自体の管理には関わりません。そのため、SPI の通信開始等を別で行い、CAN 通信を始める前に SPI でデータを送受信できるようにしておく必要があります。

  ```cpp
  static Udon::CanBusSpi bus{ SPI };
  ```

### 送信クラス

`Udon::CanWriter<T>`

`T` に指定された型のオブジェクトをバスへ送信します。

```cpp
static Udon::CanBusTeensy<CAN1> bus;
static Udon::CanWriter<Udon::Vec2> writer{ bus, 10 };  // Udon::Vec2 を bus へ ノードID 10 として送信

void setup()
{
    bus.begin();  // 通信開始
}

void loop()
{
    bus.update();

    Udon::Vec2 vector{ 100.0, 200.0 };    // 送信するオブジェクトをインスタンス化
    writer.setMessage(vector);            // オブジェクトを登録

    delay(10);
}
```

### 受信クラス

`Udon::CanReader<T>`

`T` に指定された型のオブジェクトをバスから取得します。

```cpp
static Udon::CanBusTeensy<CAN1> bus;
static Udon::CanReader<Udon::Vec2> reader{ bus, 10 };  // bus から ノードID 10 のデータ(Udon::Vec2)を受信

void setup()
{
    bus.begin();
}
void loop()
{
    bus.update();

    if (const auto/*Udon::Optional<Udon::Vec2>*/ vector = reader.getMessage())  // データ取得
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

> 通信タイムアウトを引き起こすので、常に受信し続ける必要があります。
>
> `getMessage` は正常にオブジェクトが受信できたかどうか判定できるように `Udon::Optional<T>` を返します。通信エラー時は `Udon::nullopt` が返されます。
> `Udon::Optional` は `operator bool` を持っているため if 文で正常に受信できたかどうかで分岐できます。
>
> `Udon::Optional<T>::operator->` で保持しているオブジェクトのメンバへアクセスでき、`Udon::Optional<T>::operator*` で optional が持っているオブジェクトの参照を取得できます。

### デバッグ

全 CAN 通信クラスは `show()` メンバ関数を持っており、通信の状態をシリアルモニターへ送信します。

```cpp
bus.show();     // バスに参加している送受信ノードの列挙、送受信データ(バイト列)を表示
reader.show();  // 受信データを表示
writer.show();  // 送信データを表示
```

### クラスの組み合わせ色々

一つのバスへ複数送受信ノードが参加する(よくある)

```cpp
static Udon::CanBusTeensy<CAN1> bus;
static Udon::CanWriter<Udon::Vec2> writer1{ bus, 11 };
static Udon::CanWriter<Udon::Vec2> writer2{ bus, 12 };
static Udon::CanReader<Udon::Vec2> reader1{ bus, 13 };
static Udon::CanReader<Udon::Vec2> reader2{ bus, 14 };
```

二つのバスへ受信ノードが参加する(バスの負荷分散目的)

```cpp
static Udon::CanBusTeensy<CAN1> bus1;
static Udon::CanWriter<Udon::Vec2> writer1{ bus1, 11 };
static Udon::CanReader<Udon::Vec2> reader1{ bus1, 13 };

static Udon::CanBusTeensy<CAN2> bus2;
static Udon::CanWriter<Udon::Vec2> writer2{ bus2, 12 };
static Udon::CanReader<Udon::Vec2> reader2{ bus2, 14 };
```

異なる種類のバスへ参加する(激レア)

```cpp
static Udon::CanBusTeensy<CAN1> bus1;
static Udon::CanWriter<Udon::Vec2> writer1{ bus1, 11 };
static Udon::CanReader<Udon::Vec2> reader1{ bus1, 13 };

static Udon::CanBusSpi bus2{ SPI };
static Udon::CanWriter<Udon::Vec2> writer2{ bus2, 12 };
static Udon::CanReader<Udon::Vec2> reader2{ bus2, 14 };
```
