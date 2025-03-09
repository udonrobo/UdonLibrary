# 移動平均

値の急激な変化を抑えるクラスです。

モーターの出力値を急激に変化させると、大電流が流れ、モーターやモータードライバーに負荷がかかります。移動平均を使うことで、出力値の急激な変化を抑えられます。

移動平均とは時系列データを平滑化する手法です。サンプル数が多いほど滑らかになりますが、応答性が悪くなります。

> [!NOTE]
>
> ループ毎に平均値を更新するため、ループ周期が一定でないと正しく動作しません。`LoopCycleController` を使ってループ周期を一定にすることをお勧めします。

```cpp
#include <Udon.hpp>

static Udon::LoopCycleController loopCtrl{ 10000 };
static Udon::MovingAverage<50> movingAverage;  // サンプル数 50

void setup()
{
}

void loop()
{
    movingAverage.update(入力値);
    double 出力値 = movingAverage.getValue();

    loopCtrl.update();
}
```

`operator()()` によって以下のようにも書けます。フィルターをかけるイメージです。

```cpp
void loop()
{
    double 出力値 = movingAverage(入力値);

    loopCtrl.update();
}
```

サンプル数 20 の場合、入出力の関係は以下の様になります。

<img width="600px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/7ecee0be-40ea-42df-9706-72bfa54a2713"/>
