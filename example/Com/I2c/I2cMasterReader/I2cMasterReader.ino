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
//    I2c マスター側受信クラス スケッチ例
//
//    master <--[I2C]-- slave
//    ^^^^^^
//
//-------------------------------------------------------------------

#include <Udon.hpp>

// I2C バスクラス
Udon::I2cBus bus{ Wire };

// I2C マスター側受信クラス
Udon::I2cMasterReader<Udon::Vec2> reader{ bus, 6 };

void setup()
{
    Serial.begin(115200);

    // 通信を開始します。
    bus.begin();
}

void loop()
{
    // 通信間隔を監視し、通信エラー時には再起動を行います。
    bus.update();

    // バスの状態を表示します。
    bus.show();

    // スレーブに送信リクエストを行います。
    reader.update();

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
