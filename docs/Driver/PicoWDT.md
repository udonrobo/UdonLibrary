# PicoWDT

マイコン(Raspberry Pi Pico) が落ちたときに、自動的に再起動させるクラスです。RP2040 チップ内にカウントダウンタイマーが内蔵されており、これがゼロになるとチップを再起動するという機能を用いて実装しています。

コンストラクタに最終ループから何ミリ秒後に再起動するかを指定でき、デフォルトは 1000 ミリ秒です。

```cpp
#include <Udon.hpp>

static Udon::PicoWDT wdt;

void setup()
{
}

void loop()
{
    wdt.update();

    // 起動から10秒後に無限ループに陥る -> 指定秒数後 WDT によって再起動
    if (millis() > 10000)
    {
        for (;;)
            ;
    }
}
```
