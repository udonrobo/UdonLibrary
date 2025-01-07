//
//    PS5コントローラー ペアリングスケッチ
//
//    ペアリング手順:
//        1. コンストラクタの引数 pair に PAIR(true) を渡して書き込む
//        2. コントローラーの \|/ボタン と PSボタン を同時に押し続ける
//        3. コントローラーのLEDが点滅し始める
//        4. ペアリングが完了するとLEDが点灯する
//
//    Copyright (c) 2022 udonrobo
//

#include <UdonFwd.hpp>
#include <Udon/Com/Pad/PadPS5BT.hpp>

static Udon::PadPS5BT pad(PAIR);  // <- ここを PAIR に！

void setup()
{
    Serial.begin(115200);
    if (not pad.begin())
    {
        Serial.println(F("PS5 BT failed to start!"));
        for (;;)
            ;
    }
}

void loop()
{
    pad.update();
    pad.setLightBar({ 0x38b48b });
    pad.setMicLed(true);
}
