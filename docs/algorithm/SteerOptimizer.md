# SteerOptimizer

独立ステア最適化クラス

## Description

独立ステアモジュールの旋回角、回転方向を最適化するクラス

オムニホイールの値と異なり、独立ステアの計算値はそのまま使用すると操縦性能が皆無になってしまいます。このクラスを通して最適化することで操縦性能を上げることができます。

> 最適化
>
> -   車輪が動作中でない場合に旋回角を維持する
>
>     生の計算値では車輪が動作中でない場合、車輪が常に前方に向く可能性があります。
>
>     この処理を入れることで、細かい位置調整がしすくなります。
>
> -   モジュールの旋回範囲を無限に
>
>     数式から得られる旋回角値は無限の範囲を持っていないため(atan2 であれば [ -π ~ π ]) 最大値を超過すると値が最小値に急激に変化します。この急激な変化は操作性の低下につながるため、無限の範囲に拡張します。
>
> -   車輪の回転方向の最適化
>
>     90 度以上の旋回を検出した際に、目標旋回角を 180 度旋回させ、車輪の回転方向を逆転させます。
>
>     この処理を入れることで旋回>逆旋回と切り替えたときに、モジュールを旋回せずに動作させられます。

## Usage

1. ファイルインクルード

    ```cpp
    #include <Udon.hpp>
    // or
    #include <udon/algorithm/SteerOptimizer.hpp>
    ```

2. インスタンス化

    `SteerOptimizer` クラスには車輪の個数を指定するためのテンプレート引数があります。車輪が 4 輪の場合は次のようにインスタンス化します。

    またこのクラスは前回の値を保存し比較するため、ループ毎にインスタンス化してはいけません。

    ```cpp
    // グローバル領域内など
    udon::SteerOptimizer<4> optimizer;
    ```

3. 最適化値算出

    `SteerOptimizer::operator()(const std::array<udon::Polar, WheelCount>&)` の引数に最適化前の値を入れることで、最適化後の値を戻り値から得られます。

    > このクラスでは各モジュールの値を極座標で扱います( `r` をホイールの出力値、`theta` をモジュールの旋回角に置き換えられるのため)。そのため引数、戻り値は 極座標 (`udon::Polar`) の配列を使いすべてのモジュールの値を扱います。

    > 各ステアモジュールの配置によって計算方法が異なり、汎用性を高めるため、このクラスはステアの各モジュールの値を求めることはしません。最適化前の値は各自で求めておく必要があります。
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
    // 最適化前
    const std::array<udon::Polar, 4> raw = {...};

    // 最適化後
    const std::array<udon::Polar, 4> optimized = optimizer(raw);
    ```

## Sample

CAN 通信経由のコントローラーの値からステアモジュールの値を得る例を次の示します。

```cpp
#include <Udon.hpp>

// CAN バスインスタンス化
udon::CanBus~~~ bus;

// 汎用コントローラーインスタンス化
udon::PadPS5<udon::CanReader> pad {{ bus, 10 /*node id*/ }};

// 最適化器インスタンス
udon::SteerOptimizer<4> optimizer;

void setup()
{
	bus.begin();
}

void loop()
{
	bus.update();

	// 1. コントローラから udon::Pos {{x, y}, turn} を得る `getStick()`
	// 2. udon::Pos からステアの最適化前の値を得る `toSteer()`
	// 3. 最適化後の値を得る optimizer()
	const auto optimized = optimizer(pad.getStick().toSteer());

	//... // 車輪 Node に送信するなど
}
```

OpenSiv3D でシミュレートする

UdonLibrary の追加方法は [ダウンロード: Visual Studio](./../../README.md) を参照ください。

```cpp
#include <Siv3D.hpp>

#include <udon/com/pad/PadPS5.hpp>
#include <udon/siv3d/SivPadPS5.hpp>
#include <udon/algorithm/SteerOptimizer.hpp>

void Main()
{
	udon::PadPS5<udon::SivPadPS5> pad;

	udon::SteerOptimizer<4> optimizer;

	while (System::Update())
	{
		pad.update();

		const auto optimized = optimizer(pad.getStick().toSteer());

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
