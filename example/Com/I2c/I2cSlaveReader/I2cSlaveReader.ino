//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    I2c スレーブ側受信クラス スケッチ例
//
//    master --[I2C]--> slave
//                      ^^^^^
//
//-------------------------------------------------------------------

#include <Udon.hpp>

// I2C バス管理クラス
Udon::I2cBus bus{ Wire };

// I2C スレーブ側受信クラス
Udon::I2cSlaveReader<Udon::Vec2> reader{ bus };

void setup()
{
    Serial.begin(115200);

    // 自身のアドレスを設定し、通信を開始します。
    bus.begin(6);

    // 受信を開始します。(受信割り込み設定を行います)
    reader.begin();
}

void loop()
{
    // 通信間隔を監視し、通信エラー時には再起動を行います。
    bus.update();

    // バスの状態を表示します。
    bus.show();

    // getMessage 関数によって、受信クラスのテンプレートクラス引数で指定した型の Optional 値を取得します。
    // 通信エラー時は、Udon::nullopt が返るため if 文で判定してください。
    if (const auto message = reader.getMessage())
    {
        // 正常時
        message->show();
    }
    else
    {
        // エラー時
        Serial.print("receive failed");
    }

    Serial.println();

    delay(10);
}

// 以下、シリアルモニタに表示される内容例
// restart: 1 transmit[ms]: 22346    x: 18540.00    y: 18541388.00
// restart: 1 transmit[ms]: 22358    x: 18552.00    y: 18553076.00
// restart: 1 transmit[ms]: 22369    x: 18564.00    y: 18564760.00
// restart: 1 transmit[ms]: 22381    x: 18576.00    y: 18576444.00
// restart: 1 transmit[ms]: 22381    x: 18576.00    y: 18576444.00
// restart: 1 transmit[ms]: 22393    x: 18587.00    y: 18588128.00
// restart: 1 transmit[ms]: 22404    x: 18598.00    y: 18599812.00
// restart: 1 transmit[ms]: 22416    x: 18611.00    y: 18611512.00
// restart: 1 transmit[ms]: 22428    x: 18622.00    y: 18623196.00
// restart: 1 transmit[ms]: 22439    x: 18634.00    y: 18634880.00
// restart: 1 transmit[ms]: 22451    x: 18646.00    y: 18646564.00
// restart: 1 transmit[ms]: 22451    x: 18646.00    y: 18646564.00
// restart: 1 transmit[ms]: 22463    x: 18657.00    y: 18658244.00
// restart: 1 transmit[ms]: 22474    x: 18669.00    y: 18669936.00
