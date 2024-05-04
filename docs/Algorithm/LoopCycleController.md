# ループ周期制御

ループ内の処理時間によらず、周期を一定に保ちます。

`LoopCycleController` クラスを使用します。コンストラクタ引数にループ周期をマイクロ秒単位で指定します。`update()` を呼び出すことで周期が一定に保たれます。

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

```
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
