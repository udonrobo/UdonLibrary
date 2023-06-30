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
//    I2c スレーブ側送信クラス スケッチ例
//
//    master <--[I2C]-- slave
//                      ^^^^^
//
//-------------------------------------------------------------------

#    include <Udon.hpp>

// I2C バス管理クラス
Udon::I2cBus bus{ Wire };

// I2C スレーブ側送信クラス
Udon::I2cSlaveWriter<Udon::Vec2> writer{ bus };

void setup()
{
    Serial.begin(115200);

    // 自身のアドレスを設定し、通信を開始します。
    bus.begin(6);

    // 送信を開始します。(送信割り込み設定を行います)
    writer.begin();
}

void loop()
{
    // 通信間隔を監視し、通信エラー時には再起動を行います。
    bus.update();

    // バスの状態を表示します。
    bus.show();

    // 送信データを設定します。送信クラスのテンプレート引数で指定した型の値を設定してください。
    writer.setMessage({ millis(), micros() });

    Serial.println();

    delay(10);
}

// 以下、シリアルモニタに表示される内容例
// restart: 1 transmit[ms]: 26617
// restart: 1 transmit[ms]: 26630
// restart: 1 transmit[ms]: 26643
// restart: 1 transmit[ms]: 26643
// restart: 1 transmit[ms]: 26656
// restart: 1 transmit[ms]: 26669
// restart: 1 transmit[ms]: 26682
// restart: 1 transmit[ms]: 26696
// restart: 1 transmit[ms]: 26696
// restart: 1 transmit[ms]: 26709
// restart: 1 transmit[ms]: 26722
// restart: 1 transmit[ms]: 26735
// restart: 1 transmit[ms]: 26735
// restart: 1 transmit[ms]: 26748
// restart: 1 transmit[ms]: 26761
// restart: 1 transmit[ms]: 26774
// restart: 1 transmit[ms]: 26774
// restart: 1 transmit[ms]: 26787
