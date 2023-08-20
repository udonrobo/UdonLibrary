# コントローラー

## Description

コントローラー制御クラス

## Usage

### インスタンス化

使用する通信に合わせて使用するコントローラクラスが異なります。

- PS5 コントローラ

  - I2C 通信経由

    ```cpp
    I2cPadPS5 pad{ bus, address };
    ```

  - CAN 通信経由

    ```cpp
    CanPadPS5 pad{ bus, id };
    ```

  - OpenSiv3D 使用時

    ```cpp
    SivPadPS5 pad;
    SivPadPS5 pad{ index };
    ```

### コントローラーの情報を取得する

- PS5 コントローラー

  ボタンの状態を取得する

  ```cpp
  Udon::Button getTriangle()
  Udon::Button getCircle()
  Udon::Button getCross()
  Udon::Button getSquare()
  Udon::Button getUp()
  Udon::Button getRight()
  Udon::Button getDown()
  Udon::Button getLeft()
  Udon::Button getL1()
  Udon::Button getR1()
  Udon::Button getL2()
  Udon::Button getR2()
  Udon::Button getL3()      // 左スティック押し込み
  Udon::Button getR3()      // 右スティック押し込み
  Udon::Button getCreate()  // クリエイトボタン(左上ボタン)
  Udon::Button getOption()  // オプションボタン(右上ボタン)
  Udon::Button getTouch()
  Udon::Button getPs()
  ```

  ```cpp
  const bool trianglePressed = pad.getTriangle().press;
  ```

  スティックの状態を取得する

  ```cpp
  Udon::Vec2 getLeftStick()
  Udon::Vec2 getRightStick()
  Udon::Pos  getMoveInfo()
  ```

  ```cpp
  const Udon::Vec2 leftStick   = pad.getLeftStick();
  const double     rightStickX = pad.getRightStick().x;
  const Udon::Pos  moveInfo    = pad.getMoveInfo();
  ```

## Details

ボタンを取得する関数からは、ボタンの状態を格納する構造体である `Udon::Button` オブジェクトが取得できます。

```cpp
struct Button
{
    bool press;    // 押されているか
    bool click;    // 押された瞬間か
    bool release;  // 離された瞬間か
};
```

スティックの状態を取得する関数からは、左右のスティックの `Udon::Vec2` オブジェクト、`Udon::Pos` オブジェクトが取得できます。

```cpp
struct Vec2
{
    double x;  // スティックのX成分
    double y;  // スティックのY成分
};
struct Pos
{
    Vec2 vector;  // 平行移動成分 (左スティック)
    double turn;  // 旋回成分 (右スティックのX成分)
};
```

`Udon::Pos` オブジェクトはロボットの移動操作に必要な移動成分を格納する構造体です。移動成分をすべて格納できるため、次のようなイカしたコードもかけます。

```cpp
const auto steering = pad.getMoveInfo().toSteer();
const auto omni     = pad.getMoveInfo().toOmni();
```

また、全コントローラクラスは `IPad~~` クラスを継承しており、ボタンのデータを取得する関数などはここから提供されます。
