# 独立ステア最適化

```cpp
#include <Udon/Algorithm/SteerOptimizer.hpp>
```

独立ステアモジュールの旋回角度と回転方向を最適化するためのクラス。

独立ステアの計算結果をそのまま使用すると、操縦性能が低下してしまう場合があります。このクラスを使用することで、操縦性能を向上させることができます。

> このクラスには以下の最適化処理が含まれています:
>
> - 車輪が動作中でない場合に旋回角度を維持する
>
> - 車輪の旋回範囲を無限にする
>
> - 車輪の回転方向の最適化
>
>   90 度以上の旋回が必要な場合、目標旋回角度を 180 度回転させ、車輪の回転方向を逆転させます。これにより、旋回と逆旋回の切り替え時に、車輪を旋回させずに正確な動作が可能となります。

## 全車輪最適化

`SteerOptimizer` クラスには車輪の個数を指定するためのテンプレート引数があります。車輪が 4 輪の場合は次のようにインスタンス化します。

```cpp
// グローバル領域内など
Udon::SteerOptimizer<4> optimizer;
```

`operator()` を通すことで最適値を得られます。

> 各モジュールの値を極座標で扱います。ホイールの出力値は r、モジュールの旋回角度は theta として表現されます。そのため、引数と戻り値は極座標（Udon::Polar）の配列を使用して、全てのモジュールの値を扱います。

> 注意点として、本クラスは最適化のみを行います。事前に最適化前の値を求めておく必要があります。
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

```cpp
// ループ内
// 最適化前の値を格納する配列
const std::array<Udon::Polar, 4> raw = {...};

// 最適化後
const std::array<Udon::Polar, 4> optimized = optimizer(raw);  // 最適化後の値を取得
```

## 車輪ごとに最適化

`SteerModuleOptimizer` クラスを使用することで、各車輪ごとに最適化を行えます。車輪ごとをクラスで管理している場合、こちらを使用すると便利です。

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

    // 1. コントローラから Udon::Pos {{x, y}, turn} を得る `getMoveInfo()`
    // 2. Udon::Pos からステアの最適化前の値を得る `toSteer()`
    // 3. 最適化後の値を得る optimizer()
    const auto optimized = optimizer(pad.getMoveInfo().toSteer());

    //... // 車輪 Node に送信するなど
}
```

OpenSiv3D でシミュレートする

> Visual Studio への UdonLibrary の追加方法は [ダウンロード: Visual Studio](./../../README.md) を参照してください。

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
