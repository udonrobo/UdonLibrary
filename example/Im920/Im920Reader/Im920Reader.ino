//
//    IM920 にデータを受信するサンプル
//
//    Copyright (c) 2022-2023 udonrobo
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//                                                             ^^^^^^^^
//

#include <Udon.hpp>

Udon::Im920 im920{ Serial1 };

Udon::Im920Reader<Udon::Vec2> reader{ im920 };

void setup()
{
    Serial.begin(115200);

    im920.begin(13);    // チャンネル番号を設定し、通信を開始します。
}

void loop()
{
    im920.update();

    if (const auto message = reader.getMessage())
    {
        Serial.print(message->x), Serial.print('\t');
        Serial.print(message->y), Serial.print('\n');
    }
    else
    {
        Serial.println("receive failed");    // エラー時
    }

    delay(10);
}
