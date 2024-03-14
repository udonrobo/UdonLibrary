//
//    エンコーダーのカウント値を取得する例 (Raspberry Pi Pico)
//
//    Copyright (c) 2022-2023 udonrobo
//

#include <Udon.hpp>

Udon::EncoderPico encoder{ 0, 1 };

void setup()
{
    encoder.begin();
}

void loop()
{
    Serial.println(encoder.read());
    delay(10);
}
