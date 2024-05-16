//
//    IM920 からデータを送信するサンプル
//
//    Copyright (c) 2022-2024 udonrobo
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//    ^^^^^^
//

#include <Udon.hpp>

Udon::Im920 im920{ Serial1 };

Udon::Im920Writer<Udon::Vec2> writer{ im920 };

void setup()
{
    im920.begin(13);    // チャンネル番号を設定し、通信を開始します。
}

void loop()
{
    writer.setMessage({ (double)millis(), (double)micros() });
    im920.update();

    delay(10);
}
