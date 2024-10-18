# メッセージ型

送受信するデータの型は `UdonLibrary/Com/Message/` 下に定義されています。またユーザー定義型の一部もメッセージとして使用できます。

## メッセージ型一覧

アクチュエータ系

| メッセージ型                 | 説明               | メンバ              |
| ---------------------------- | ------------------ | ------------------- |
| `Udon::Message::Servo`       | サーボ             | `angle: uint16_t`   |
| `Udon::Message::Motor`       | モータ             | `power: int16_t`    |
| `Udon::Message::BrushLess`   | ブラシレスモータ   | `power: int32_t`    |
| `Udon::Message::Stepping`    | ステッピングモータ | `position: int32_t` |
| `Udon::Message::AirCylinder` | エアシリンダ       | `push: bool`        |
| `Udon::Message::Solenoid`    | ソレノイド         | `push: bool`        |

その他・センサ系

| メッセージ型                     | 説明             | メンバ                       |
| -------------------------------- | ---------------- | ---------------------------- |
| `Udon::Message::Switch`          | スイッチ         | `press: bool`                |
| `Udon::Message::EmergencySwitch` | 緊急停止スイッチ | `press: bool`                |
| `Udon::Message::Encoder`         | エンコーダ       | `count: int32_t`             |
| `Udon::Message::PhotoSensor`     | フォトセンサ     | `light: bool`                |
| `Udon::Message::DistanceSensor`  | 距離センサ       | `distance: double`           |
| `Udon::Message::Yaw`             | IMU yaw角        | `yaw: double`                |
| `Udon::Message::Lcd`             | LCD              | `text: char[Row][Column]`    |
| `Udon::Message::PadPS5`          | PS5 コントローラ | 各種ボタン、スティックの情報 |

ユーザー定義型

| メッセージ型       | 説明           | メンバ                                       |
| ------------------ | -------------- | -------------------------------------------- |
| `Udon::Vec2`       | 2 次元ベクトル | `x: double` `y: double`                      |
| `Udon::Vec3`       | 3 次元ベクトル | `x: double` `y: double` `z: double`          |
| `Udon::Euler`      | オイラー角     | `roll: double` `pitch: double` `yaw: double` |
| `Udon::Polar`      | 極座標         | `r: double` `theta: double`                  |
| `Udon::Pos`        | 位置           | `x: double` `y: double` `z: double`          |
| `Udon::Quaternion` | クォータニオン | `vector: Vec2` `turn: double`                |
| `Udon::RGB`        | RGB 色空間     | `r: uint8_t` `g: uint8_t` `b: uint8_t`       |
| `Udon::HSV`        | HSV 色空間     | `h: uint8_t` `s: uint8_t` `v: uint8_t`       |

## 新たにメッセージ型を定義する

メッセージ型は構造体として定義されています。例えば 2 次元ベクトルは次のように定義されています。`UDON_PARSABLE` マクロを使用してメンバ変数を列挙できるようにする必要があります。(シリアライザ等が使用します)

```cpp
struct Vector2D
{
    double x;
    double y;

    UDON_ENUMERABLE(x, y)
};
```

ライブラリに定義を追加する場合、`Udon::Message` 名前空間内に定義し、`UdonLibrary/Com/Message/` 下に配置します。また、`UdonLibrary/Com/Message/Message.hpp` にインクルードを追加します。

```cpp
// UdonLibrary/Com/Message/Vector2D.hpp

namespace Udon
{
    namespace Message
    {
        struct Vector2D
        {
            double x;
            double y;

            UDON_ENUMERABLE(x, y)
        };
    }
}
```

```cpp
// UdonLibrary/Com/Message/Message.hpp

#include <Udon/Com/Message/Vector2D.hpp>
```
