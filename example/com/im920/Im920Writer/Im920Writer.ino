//-----------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-----------------------------------------------
//
//    IM920 送信クラス スケッチ例
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//    ^^^^^^
//
//-----------------------------------------------

#ifdef HAVE_HWSERIAL1

#    include <Udon.hpp>

// IM920 引数にはIm920と通信するシリアルクラスのインスタンスを指定します。
udon::Im920 im920{ Serial1 };

// IM920 送信クラス クラステンプレート引数には送信データの型を指定します。
udon::Im920Writer<udon::Vec2> writer{ im920 };

void setup()
{
    Serial.begin(115200);

    // IDを設定し、通信を開始します。
    im920.begin(13);
}

void loop()
{

    // 通信の更新を行います
    im920.update();

    // 通信状態を表示します
    im920.show();

    // 送信データを設定します。送信クラスのテンプレート引数で指定した型の値を設定してください。
    writer.setMessage({ (double)millis(), (double)micros() });

    Serial.println();

    // IM920 は送信に 3.2ms＋160μs×バイト の処理時間がかかります。
    delay(10);
}

#else
void setup() {}
void loop() {}
#endif
