//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  I2c マスター側受信クラス スケッチ例
//
//-----------------------------------------------

#include <Udon.hpp>

// I2C バス管理クラス
udon::I2cBus bus{ Wire };

// I2C マスター側受信クラス
udon::I2cMasterReader<udon::Vec2> reader{ bus, 6 };

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

    // getMessage 関数によって、受信クラスのテンプレートクラス引数で指定した型の optional 値を取得します。
    // 通信エラー時は、udon::nullopt が返るため if 文で判定してください。
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
// restart: 1 transmit[ms]: 32890   x: 1631.00    y: 1631234.00
// restart: 1 transmit[ms]: 32903   x: 1651.00    y: 1651239.00
// restart: 1 transmit[ms]: 32916   x: 1661.00    y: 1661240.00
// restart: 1 transmit[ms]: 32929   x: 1671.00    y: 1671243.00
// restart: 1 transmit[ms]: 32942   x: 1681.00    y: 1681244.00
// restart: 1 transmit[ms]: 32955   x: 1701.00    y: 1701248.00
// restart: 1 transmit[ms]: 32968   x: 1711.00    y: 1711251.00
// restart: 1 transmit[ms]: 32980   x: 1721.00    y: 1721253.00
// restart: 1 transmit[ms]: 32994   x: 1741.00    y: 1741257.00
// restart: 1 transmit[ms]: 33007   x: 1751.00    y: 1751258.00
// restart: 1 transmit[ms]: 33019   x: 1761.00    y: 1761261.00
// restart: 1 transmit[ms]: 33033   x: 1771.00    y: 1771263.00
// restart: 1 transmit[ms]: 33046   x: 1791.00    y: 1791266.00
// restart: 1 transmit[ms]: 33059   x: 1801.00    y: 1801268.00
// restart: 1 transmit[ms]: 33072   x: 1811.00    y: 1811271.00
// restart: 1 transmit[ms]: 33085   x: 1831.00    y: 1831275.00
