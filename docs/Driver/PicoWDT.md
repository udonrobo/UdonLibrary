# PicoWDT

マイコン (Raspberry Pi Pico) が落ちたときに、自動的に再起動させるクラスです。RP2040 チップ内にはオーバーフローするとマイコン自体をリセットするタイマが実装されており、それを利用しています。

コンストラクタに最終ループから何ミリ秒後に再起動するかを指定でき、デフォルトは 1000 ミリ秒です。

```cpp
#include <Udon.hpp>

static Udon::PicoWDT wdt;

void setup()
{
}

// 起動から10秒後に無限ループに陥る関数
void deadLater()
{
    if (millis() > 10000)
        for (;;)
            ;
}

void loop()
{
    wdt.update();

    deadLater();
}
```
