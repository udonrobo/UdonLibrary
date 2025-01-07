//
//    I2C スレーブ側受信スケッチ例
//
//    Copyright (c) 2022 udonrobo
//

#include <Udon.hpp>

static Udon::I2cBus bus{ Wire };

static Udon::I2cSlaveReader<Udon::Vec2> reader{ bus };

void setup()
{
    Serial.begin(115200);

    bus.begin(6);      // 自身のアドレスを設定し、通信を開始します。
    reader.begin();    // 受信を開始します。(受信割り込み設定を行います)
}

void loop()
{
    bus.update();

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
