# ループ周期制御

ループ周期を一定に保つクラスです。

`loop()` 関数の呼び出し周期は、処理時間によって変動します。PID 制御のように一定周期でのフィードバックが重要なアルゴリズムを使用する場合、このクラスを併用します。

コンストラクタ引数にループ周期(マイクロ秒)を指定し、`update()` を呼び出すことで周期が一定に保たれます。

```cpp
#include <Udon.hpp>

static Udon::LoopCycleController loopCtrl{ 10000 };

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    Serial.println(millis());

    delay(random(9));  // 0 ~ 9 ミリ秒の遅延を生成

    loopCtrl.update();
}
```

```txt
10
20
30
40
50
...
```

`cycleUs()` でコンストラクタで指定したループ周期を取得できます。

```cpp
const auto cycleUs = loopCtrl.cycleUs();
```
