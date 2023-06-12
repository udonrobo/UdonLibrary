# SteerOptimizer

独立ステア最適化クラス

## Description

SteerOptimizer は、独立ステアモジュールの旋回角度と回転方向を最適化するためのクラスです。独立ステアの計算結果をそのまま使用すると、操縦性能が低下してしまう場合があります。このクラスを使用することで、操縦性能を向上させることができます。

> このクラスには以下の最適化処理が含まれています:
>
> -   車輪が動作中でない場合に旋回角度を維持する
>
>     生の計算値では、車輪が動作中でない場合にモジュールが常に前方を向く傾向があります。この最適化処理を適用することで、細かな位置調整が容易になります。
>
> -   モジュールの旋回範囲を無限に
>
>     数式から得られる旋回角度は通常、有限の範囲で表現されます（例えば、atan2 関数の場合は[-π, π]）。しかし、値が最大値を超えると急激に最小値に変化してしまうため、操作性が低下する可能性があります。この最適化処理により、旋回範囲を無限に拡張し、滑らかな制御が可能となります。
>
> -   車輪の回転方向の最適化
>
>     90 度以上の旋回が必要な場合、目標旋回角度を 180 度回転させ、車輪の回転方向を逆転させます。これにより、旋回と逆旋回の切り替え時に、モジュールを旋回させずに正確な動作が可能となります。

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

    `SteerOptimizer` クラスの operator()メンバ関数に最適化前の値を引数として渡すことで、最適化後の値を戻り値として取得することができます。

    > このクラスでは、各モジュールの値を極座標で扱います。ホイールの出力値は r、モジュールの旋回角度は theta として表現されます。そのため、引数と戻り値は極座標（udon::Polar）の配列を使用して、全てのモジュールの値を扱います。

    > 注意点として、SteerOptimizer クラスは各モジュールの値を直接求めるのではなく、最適化処理を行うためのクラスです(各ステアモジュールの配置によって計算方法が異なるため) したがって、事前に最適化前の値を準備しておく必要があります。
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
    const std::array<udon::Polar, 4> raw = {...};

    // 最適化後
    const std::array<udon::Polar, 4> optimized = optimizer(raw);  // 最適化後の値を取得
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

	// 1. コントローラから udon::Pos {{x, y}, turn} を得る `getMovementInfo()`
	// 2. udon::Pos からステアの最適化前の値を得る `toSteer()`
	// 3. 最適化後の値を得る optimizer()
	const auto optimized = optimizer(pad.getMovementInfo().toSteer());

	//... // 車輪 Node に送信するなど
}
```

OpenSiv3D でシミュレートする

> Visual Studio への UdonLibrary の追加方法は [ダウンロード: Visual Studio](./../../README.md) を参照してください。

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

		const auto optimized = optimizer(pad.getMovementInfo().toSteer());

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
