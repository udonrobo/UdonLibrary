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
//    エンコーダーのカウント値を取得する例 (Raspberry Pi Pico)
//
//-------------------------------------------------------------------

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
