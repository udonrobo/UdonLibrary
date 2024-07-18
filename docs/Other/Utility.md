# ユーティリティ

## `Show 関数`

様々なオブジェクトを出力できます。マルチプラットフォームで動作します。PC の場合標準出力、マイコンの場合 USB シリアルへ送信します。

```cpp
void setup()
{
    Serial.begin(115200);  // マイコンを使用する場合必要
    Udon::Show("Hello world!");
}
```

ユーザー定義型の出力

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
}
```

Show

ShowRaw 

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

## `PicoWDT クラス`

マイコン(Raspberry Pi Pico) が落ちたときに、自動的に再起動する機能です。

コンストラクタに最終ループから何ミリ秒後に再起動するかを指定できます。デフォルトは 1000 ミリ秒です。

```cpp
#include <Udon.hpp>

static Udon::PicoWDT wdt;

void setup()
{
}

void loop()
{
    wdt.update();

    // 起動から10秒後に無限ループに陥る -> 指定秒数後、WDT によって再起動
    if (millis() > 10000)
    {
        for (;;)
            ;
    }
}
```
