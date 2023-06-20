//-----------------------------------------------
//
//	UdonLibrary
//
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  I2c マスター側送信クラス スケッチ例
//
//-----------------------------------------------

#ifdef ARDUINO

#    include <Udon.hpp>

// I2C バス管理クラス
udon::I2cBus bus{ Wire };

// I2C マスター側送信クラス
udon::I2cMasterWriter<udon::Vec2> writer{ bus, 6 };

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

    // 送信データを設定します。送信クラスのテンプレートクラス引数で指定した型の値を設定してください。
    writer.setMessage({ millis(), micros() });

    Serial.println();

    delay(10);
}

// 以下、シリアルモニタに表示される内容例
// restart: 0 transmit[ms]: 49227
// restart: 0 transmit[ms]: 49239
// restart: 0 transmit[ms]: 49251
// restart: 0 transmit[ms]: 49262
// restart: 0 transmit[ms]: 49274
// restart: 0 transmit[ms]: 49286
// restart: 0 transmit[ms]: 49298
// restart: 0 transmit[ms]: 49309
// restart: 0 transmit[ms]: 49320
// restart: 0 transmit[ms]: 49333
// restart: 0 transmit[ms]: 49344
// restart: 0 transmit[ms]: 49356
// restart: 0 transmit[ms]: 49368
// restart: 0 transmit[ms]: 49379
// restart: 0 transmit[ms]: 49391
// restart: 0 transmit[ms]: 49402

#endif