# 汎用的な関数

## `Assert()`

```cpp
Assert(bool expression, const char* const message = "", AssertAction action = AssertAction::Abort)
```

`expression` が false となるとき、任意のメッセージを出力しプログラムを中断する関数です。`action` に `Udon::AssertAction::Skip` を指定することで中断しないように設定できます。使用例:

```cpp
#include <Udon.hpp>

static Udon::PadPS5BT pad;

void setup()
{
    Udon::Assert(pad.begin(), "PS5 BT failed to start!");
}
```

## `Normalized()`

```cpp
double Normalized(double value, double min, double max)
```

-∞~+∞ の範囲を min~max の範囲に変換するします。最小値が設定できるようになった剰余算のイメージです。

```cpp
Normalized(190, -180, 180) == 10
Normalized(-200, -180, 180) == -20
```
