//
//    I2C マスター側受信スケッチ例
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Udon.hpp>

Udon::I2cBus bus{ Wire };

Udon::I2cMasterReader<Udon::Vec2> reader{ bus, 6 };

void setup()
{
    Serial.begin(115200);
    bus.begin();
}

void loop()
{
    bus.update();

    reader.update();    // スレーブに送信リクエストを行います。
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
