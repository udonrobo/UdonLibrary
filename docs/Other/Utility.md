# ユーティリティ

## `Show 関数`

様々なオブジェクトを出力でき、マルチプラットフォームで動作します。PC の場合標準出力、マイコンの場合 USB シリアルへ送信します。また `Udon::Showln` 関数を用いると最後に改行を出力します。

```cpp
void setup()
{
    Serial.begin(115200);  // マイコンを使用する場合必要
    Udon::Show("Hello world!");
}
```

### ユーザー定義型の出力

```cpp
struct Sample
{
    int i;
    double d;
    UDON_ENUMERABLE(i, d);
};

void setup()
{
    Serial.begin(115200);
    Udon::Show(Sample{ 10, 20.0 });
    Udon::Show(Sample{ 10, 20.0 });
}
```

```
{ 10, 20 }{ 10, 20 }
```

## `Assert 関数`

```cpp
Assert(bool expression, const char* const message = "", AssertAction action = AssertAction::Abort)
```

`expression` が false となるとき、任意のメッセージを出力しプログラムを中断する関数です。`action` に `Udon::AssertAction::Skip` を指定することで中断しないように設定できます。

```cpp
#include <Udon.hpp>

static Udon::PadPS5BT pad;

void setup()
{
    Serial.begin(115200);
    Udon::Assert(pad.begin(), "PS5 BT failed to start!");
}
```

## `Normalized 関数`

```cpp
double Normalized(double value, double min, double max)
```

-∞~+∞ の範囲を min~max の範囲に変換するします。最小値が設定できるようになった剰余算のイメージです。

```cpp
Normalized(190, -180, 180) == 10
Normalized(-200, -180, 180) == -20
```
