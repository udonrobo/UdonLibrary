# フィードバック制御

フィードバック制御は、機構の位置制御、速度制御等に用いられる制御方法です。

例えば昇降機構をある高さに移動させる場合を考えます。モーター単体では、機構が現在どの位置にいるか把握することができないため、目標となる高さに移動できません。そこでロータリーエンコーダーなどの現在位置を取得するセンサを用い、目標となる高さと現在位置を比較し、モータのパワーを調整することで目標の高さへ到達させます。これがフィードバック制御です。

フィードバック制御にはいくつか手法がありますが、本部活では昔から使用されている調整が手軽な PID 制御を用います。

## PidController

PID 制御を行うには `PidController` クラスを用います。このクラスはループ周期が一定である前提で作成されているため、`LoopCycleController` クラスも必要です。

PID 制御には 3 つのパラメータ (P ゲイン, I ゲイン, D ゲイン) があり、コンストラクタで設定します。このパラメータがフィードバック制御の性能を決定します。

`PidController::update()` に現在の値、目標の値を渡すことで `PidController::getPower()` から制御値が得られます。この制御値用いてモータを動かすことで位置制御などが可能です。現在値にはエンコーダーなどのセンサから得られる値を設定します。

```cpp
#include <Udon.hpp>

static Udon::LoopCycleController loopCtrl{ 10000 };  // ループ周期 10000マイクロ秒

static Udon::PidController pid{ 2, 0.1, 1, loopCtrl.cycleUs() };
//                              ^  ^^^  ^
//                              P   I   D

void setup()
{
}

void loop()
{
    pid.update(現在値, 目標値);
    double 制御値 = pid.getPower();

    loopCtrl.update();
}
```

`operator()()` によって以下のようにも書けます。

```cpp
void loop()
{
    double 制御値 = pid(現在値, 目標値);

    loopCtrl.update();
}
```

エンコーダー、モータがあると仮定すると以下のようなスケッチとなります。この例ではエンコーダーのカウント値が 10000 となるようにモータのパワーを調整します。

```cpp
void loop()
{
    // encoder --> PID --> motor
    const int32_t count = encoder.getCount();
    const int32_t target = 10000;
    
    const auto power = pid(count, target);

    motor.move(power);

    loopCtrl.update();
}
```

積分定数が設定されている場合、非常停止中に誤差が積算されていき、非常停止解除時に急に動く場合があります。`clearPower()` メンバ関数で積算値の消去を行えます。

```cpp
if (非常停止中)
{
    pid.clearPower();
}
```
