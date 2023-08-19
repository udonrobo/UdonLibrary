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

// I2C バスクラス
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
