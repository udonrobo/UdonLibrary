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
//    IM920 受信クラス スケッチ例
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//                                                             ^^^^^^^^
//
//-----------------------------------------------

#ifdef HAVE_HWSERIAL1

#    include <Udon.hpp>

// IM920 引数にはIm920と通信するシリアルクラスのインスタンスを指定します。
udon::Im920 im920{ Serial1 };

// IM920 受信クラス クラステンプレート引数には受信データの型を指定します。
udon::Im920Reader<udon::Vec2> reader{ im920 };

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

#else
void setup() {}
void loop() {}
#endif