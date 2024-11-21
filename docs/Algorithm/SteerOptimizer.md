# 独立ステア最適化

数式から得られた独立ステアの出力値をそのまま使用すると、かなり操縦の難しい足回りとなってしまいます。`SteerOptimizer` クラスはを使用することで、操縦性向上が見込めます。以下の最適化処理が含まれています。

- 車輪が動作中でない場合に旋回角度を維持する

  <img width="400px" src="https://github.com/user-attachments/assets/298c4f20-9b63-423d-ba46-f87974e8de11"/>

- 車輪の旋回範囲を無限にする

  式から得られた旋回角度は、±π の範囲で得られます。この範囲を無限に拡張します。

  ゆっくりモジュールを旋回しようとした場合、次のような挙動をします。最適化しない場合 π と -π の境界を超える時に、逆方向に旋回してしまいます。

  <img width="400px" src="https://github.com/user-attachments/assets/d100e091-2295-4800-9e2e-7fc83b3039c8"/>

- 車輪の回転方向の最適化

  90 度以上の旋回が必要な場合、目標旋回角度を 180 度回転させ、車輪の回転方向を逆転させます。目標角度へ最短で到達できるようになります。

  <img width="400px" src="https://github.com/user-attachments/assets/9570c557-7f15-4bed-a39d-6f3a844e50ba"/>

## 個別インクルード

```cpp
#include <Udon/Algorithm/SteerOptimizer.hpp>
```

## 全車輪最適化

`SteerOptimizer` クラスを用います。このクラスには車輪数を指定するためのテンプレート引数があります。下記の例では 4 つの車輪を使用しています。`operator()` を通すことで最適値を得られます。

```cpp
static Udon::SteerOptimizer<4> optimizer;

void setup()
{
    ...
}

void loop()
{
    // 移動量
    Udon::Pos pos = { { 0.0, 100.0 }, 0.0 };  // y 軸方向に 100 進行

    // 最適化前
    std::array<Udon::Polar, 4> raw = pos.toSteer();

    // 最適化後
    std::array<Udon::Polar, 4> optimized = optimizer(raw);  // optimizer.operator()

    for (auto&& polar : optimized)
    {
        Serial.print(polar.r);      // ホイールの出力値 [-255 ~ 255]
        Serial.print(" ");
        Serial.print(polar.theta);  // モジュールの旋回角度 [rad]
        Serial.print(" ");
    }
    Serial.println();

}
```

> 本クラスは最適化のみを行うため、事前に最適化前の値を求めておく必要があります。上記の例では `Udon::Pos::toSteer()` で求めています。
>
> 最適化前の値要件:
>
> - `r` [ -π ~ π ] ラジアン値
>
> - `theta` [ 自由 ]
>
> 最適化後の値:
>
> - `r` [ -∞ ~ ∞ ] ラジアン値
>
> - `theta` [ ± 最適化前 `theta` ]

## 車輪ごとに最適化

実は、車輪の最適化は独立して計算されます(車輪間で影響し合わない)。そのため、`SteerModuleOptimizer` クラスを使用することで、各車輪ごとに最適化を行えます。車輪ごとをクラスで管理している場合、こちらを使用すると便利です。

```cpp
Udon::SteerModuleOptimizer optimizer;

Udon::Polar raw = {...};

Udon::Polar optimized = optimizer(raw);
```

## Sample

CAN 通信経由のコントローラーの値からステアモジュールの値を得る例を次の示します。

```cpp
#include <Udon.hpp>

// CAN バスインスタンス化
Udon::CanBus~~~ bus;

// コントローラーインスタンス化
Udon::CanPadPS5 pad { bus, 10 /*node id*/ };

// 最適化器インスタンス
Udon::SteerOptimizer<4> optimizer;

void setup()
{
    bus.begin();
}

void loop()
{
    bus.update();

    // 1. `getMoveInfo()`: コントローラから Udon::Pos {{x, y}, turn} を得る
    // 2. `toSteer()`    : Udon::Pos からステアの最適化前の値を得る
    // 3. `optimizer()`  : 最適化後の値を得る
    const auto optimized = optimizer(pad.getMoveInfo().toSteer());

    //... // 車輪 Node に送信するなど
}
```

## OpenSiv3D でシミュレートする

> Visual Studio への UdonLibrary の追加方法は [追加方法: Visual Studio](./../../README.md) を参照してください。

```cpp
#include <Siv3D.hpp>
#include <Udon.hpp>

void Main()
{
    Udon::SivPadPS5 pad;

    Udon::SteerOptimizer<4> optimizer;

    while (System::Update())
    {
        pad.update();

        const auto optimized = optimizer(pad.getMoveInfo().toSteer());

        const RectF frame { Arg::center = Scene::CenterF(), 300, 300 };
        frame.drawFrame();

        const auto stretchedFrame = frame.stretched(-50);

        const auto lines = Array{
            Line{ stretchedFrame.point(1), Arg::angle = optimized.at(0).theta, optimized.at(0).r + 0.000000001 },
            Line{ stretchedFrame.point(2), Arg::angle = optimized.at(1).theta, optimized.at(1).r + 0.000000001 },
            Line{ stretchedFrame.point(3), Arg::angle = optimized.at(2).theta, optimized.at(2).r + 0.000000001 },
            Line{ stretchedFrame.point(0), Arg::angle = optimized.at(3).theta, optimized.at(3).r + 0.000000001 }
        };

        for (auto&& line : lines)
        {
            RectF{ Arg::center = line.begin, 30, 60 }
                .rotated(line.vector().getAngle())
                .drawFrame();
            line.drawArrow();
        }

    }
}
```
