# 7 セグメント LED

`SegmentsLed` クラスを用いて、複数桁の 7 セグメント LED を制御する方法を説明します。本クラスはアノードコモンの 7 セグメント LED 専用です。

## 個別インクルード

```cpp
#include <Udon/Driver/SegmentsLed.hpp>
```

## インスタンス化

コンストラクタにピン番号を指定します。第一引数にはカソードを接続するピン番号を、第二引数にはアノードを接続するピン番号を指定します。

```cpp
static Udon::SegmentsLed segmentsLed {
    {{ 4, 2, 9, 8, 7, 3, 10 }},
    { 6, 5 },
};
```

カソードピンの順と、LED の配置は次のように対応しています。回路図を参考にピン番号を指定してください。

```cpp
{{ a, b, c, d, e, f, g }}
```

```txt
   a
f     b
   g
e     c
   d
```

アノードピンは小さい桁から順に指定します。最初に指定したピン番号が最下位桁に対応します。桁数はここで指定するピン番号の数によって決まります。上記の例では 2 桁です。

```cpp
{ 1桁目アノード, 2桁目アノード, ... }
```

## 値表示

`begin` 関数を呼び出し、初期化を行った後、`setValue` 関数を用いて値を設定します。`show` 関数を呼び出すことで、設定した値が表示されます。

本クラスはダイナミック点灯で複数桁の値を表示します。ループ毎に表示する桁を切り替えるため、ループが停止すると表示が乱れます。またループ周期が早すぎる場合、表示が薄くなることがあります。

```cpp
#include <Udon.hpp>

static Udon::SegmentsLed segmentsLed {
    {{ 4, 2, 9, 8, 7, 3, 10 }},
    { 6, 5 },
};

void setup()
{
    segmentsLed.begin();
    segmentsLed.setValue(10);
}

void loop()
{
    segmentsLed.show();
    delay(1);
}
```

## 無効値表示

`showNan` 関数を用いると無効値(`--`)を表示出来ます。通信エラーやセンサーの故障など、値が取得できない場合に使用することを想定しています。

```cpp
#include <Udon.hpp>

static Udon::SegmentsLed segmentsLed {
    {{ 4, 2, 9, 8, 7, 3, 10 }},
    { 6, 5 },
};

void setup()
{
    segmentsLed.begin();
}

void loop()
{
    segmentsLed.showNan();
}
```
