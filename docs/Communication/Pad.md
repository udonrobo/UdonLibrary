# PS5 コントローラー

`PadPS5`クラスを用いることでメインマイコンから PS5 コントローラの情報を取得出来ます。

このクラスはテンプレートクラスになっており、受信クラスを指定します。CAN、I2C、E220、USB、Bluetooth などの通信方式に対応しています。

## 個別インクルード

```cpp
#include <Udon/Com/Pad.hpp>
```

## インスタンス化

`PadPS5`クラスはテンプレートクラスになっており、引数に受信クラスを指定することで通信方式を選択できます。

引数に指定した受信クラスを`PadPS5`クラスが継承するため、受信クラスのメンバ関数をそのまま使用できます。同様にコンストラクタも継承されます。受信クラスのテンプレート引数には `Message::PadPS5` が指定されます。

例えば CAN 経由でコントローラの情報を取得する場合、以下のように記述できます。

```cpp
static Udon::CanBusTeensy<CAN1> bus;
static Udon::PadPS5<Udon::CanReader> pad{ bus, id };
```

記述が長いため、エイリアスを使用することをお勧めします。

```cpp
static Udon::CanBusTeensy<CAN1> bus;
static Udon::CanPadPS5 pad{ bus, id };
```

<details>
<summary>その他の通信方式でのエイリアス</summary>

- I2C

  ```cpp
  static Udon::I2cBus bus{ Wire };
  static Udon::I2cPadPS5 pad{ bus, address };
  ```

- E220 (無線モジュール)

  ```cpp
  static Udon::E220PadPS5 pad({
      .serial = Serial1,
      .m0 = 2,
      .m1 = 3,
      .aux = 4,
  });
  ```

- メインマイコンと USB ホストシールドが直接接続されている場合 (Bluetooth 使用時)

  Udon.hpp とは別に PadPS5BT.hpp にインクルードする必要があります(依存ライブラリのサイズが大きいため)

  ```cpp
  #include <Udon.hpp>
  #include <Udon/Com/Pad/PadPS5BT.hpp>

  static Udon::PadPS5OnboardBT pad;
  ```

- メインマイコンと USB ホストシールドが直接接続されている場合 (USB 使用時)

  ```cpp
  #include <Udon.hpp>
  #include <Udon/Com/Pad/PadPS5USB.hpp>

  static Udon::PadPS5OnboardUSB pad;
  ```

- OpenSiv3D を使用している場合

  > PC と USB ケーブルで接続すると使用できます。

  ```cpp
  static SivPadPS5 pad;
  // static SivPadPS5 pad{ index };  // 複数コントローラを使用する場合
  ```

</details>

## 通信開始 (Optional)

本クラスは通信の管理は行わず、受信クラスから受け取ったデータの解析のみを行います。故に `begin` メンバ関数はありません。通信の開始処理が必要な受信クラスを使用している場合、通信開始処理を行う必要があります。

```cpp
void setup()
{
    pad.begin();  // 受信クラスに開始処理を行うメンバ関数がある場合
}
```

> `PadPS5` クラスに `begin` メンバ関数がないのに呼び出せているのは、`PadPS5` クラスが受信クラスを継承していて、受信クラスの `begin` メンバ関数を引き継いでいるためです。

## 更新 (必須)

ループ内で `update` メンバ関数を呼ぶ必要があります。受信クラスに `update` メンバ関数が実装されている場合、自動的に呼び出されます。

```cpp
void loop()
{
    pad.update();
}
```

## 遠隔非常停止の実装

ロボコンのルール上、遠隔非常停止を実装する必要があります。他を顧みず実装するなら以下のように記述できます。コントローラが接続されているかは `operator bool` によって取得できます。

```cpp
void loop()
{
    pad.update();
    if (pad and pad.getCross().toggle) // 接続されている、×ボタンを押し非常停止が解除された場合
    {
        omni.move(stick);
    }
    else
    {
        omni.stop();
    }
}
```

bool 値を複数扱うと頭が混乱するため、`PadPS5` クラスは非常停止を行うべきかを判断するための `isEmergencyStop`、また逆の動作可能かを判断するための `isOperable` メンバ関数を提供しています。内部的には上記の処理と同等です。

```cpp
void loop()
{
    pad.update();
    if (pad.isOperable())
    {
        omni.move(stick);
    }
    else
    {
        omni.stop();
    }
}
```

## ボタン

次のようにボタンの状態を `Input` オブジェクトとして取得できます。

```cpp
void loop()
{
    pad.update();
    Udon::Input input = pad.getTriangle();
    Udon::Input input = pad.getCircle();
    Udon::Input input = pad.getCross();
    Udon::Input input = pad.getSquare();
    Udon::Input input = pad.getUp();
    Udon::Input input = pad.getRight();
    Udon::Input input = pad.getDown();
    Udon::Input input = pad.getLeft();
    Udon::Input input = pad.getL1();
    Udon::Input input = pad.getR1();
    Udon::Input input = pad.getL2();
    Udon::Input input = pad.getR2();
    Udon::Input input = pad.getL3();      // 左スティック押し込み
    Udon::Input input = pad.getR3();      // 右スティック押し込み
    Udon::Input input = pad.getCreate();  // クリエイトボタン(左上ボタン)
    Udon::Input input = pad.getOption();  // オプションボタン(右上ボタン)
    Udon::Input input = pad.getTouch();
    Udon::Input input = pad.getPs();
}
```

> `Input` オブジェクトは以下の様に定義されており、各ボタンの押された瞬間、離した瞬間等を取得できます。
>
> ```cpp
> struct Input
> {
>     bool press;    // 押されているか
>     bool click;    // 押された瞬間か
>     bool release;  // 離された瞬間か
>     bool toggle;   // 押すごとに入れ替わる
> };
> ```
>
> ```cpp
> // 三角ボタンが押されているか
> const bool trianglePressed = pad.getTriangle().press;
> ```

## スティック

以下の関数から左右のスティックの値を `Vec2` オブジェクトして取得可能です。Vec2 クラスの詳細は [こちら](../Types/Vector2D.md)

```cpp
void loop()
{
    pad.update();
    Udon::Vec2 left  = pad.getLeftStick();
    Udon::Vec2 right = pad.getRightStick();
}
```

> `Vec2` オブジェクトは以下の様に定義されており、各スティックの X, Y 軸の傾きを取得できます。スティックの上方向が X 軸の正、右方向が Y 軸の正です。
>
> ```cpp
> struct Vec2
> {
>     double x;  // -255 ~ 255
>     double y;  // -255 ~ 255
> };
> ```
>
> ```cpp
> // 左スティックのx軸の傾き
> const double leftX = pad.getLeftStick().x;
> ```

また、`Stick` オブジェクトを取得できる関数もあります。`Stick` オブジェクトとはロボットの移動に必要な入力成分 (移動: 左 X,Y、旋回: 右 X) をまとめたオブジェクトで、このオブジェクトを用いてオムニホイールの出力値を求めたりすることもできます。

```cpp
Udon::Stick stick = pad.getMoveInfo();
```

> `Stick` オブジェクトは以下の様に定義されています。
>
> ```cpp
> struct Stick
> {
>     Vec2 vector;  // 平行移動成分 (左スティック)
>     double turn;  // 旋回成分 (右スティックのX成分)
> };
> ```
>
> ```cpp
> // スティックからオムニの出力値を求める
> const std::array<double, 4> omni = pad.getMoveInfo().toOmni();
> ```

## 最終的なスケッチ例 (CAN バス経由)

```cpp
#include <Udon.hpp>

static Udon::CanBusTeensy<CAN1> bus;
static Udon::CanPadPS5 pad{ bus, 0x006 };

void setup()
{
    Serial.begin(115200);
    bus.begin();
}

void loop()
{
    bus.update();
    pad.update();

    if (pad.getCircle().click)
    {
        Serial.println("circle clicked");
    }

    delay(1);
}

```

## メインマイコンと USB ホストシールドが直接接続されている場合

コントローラに内蔵されている LED や、バイブレーションを動作させられます。

```cpp
#include <Udon.hpp>

static Udon::PadPS5OnboardBT pad;

void setup()
{
    pad.begin();
}

void loop()
{
    pad.update();
    pad.setLightBar({ 0x38b48b });   // タッチパネルサイドLED (色指定可能)
    pad.setMicLed(true);             // マイクLED
    pad.setPlayerLamp();             // タッチパネル下部LED (5つ)
    pad.vibrate(100, 100);           // 左右バイブレーションモーター
}
```
