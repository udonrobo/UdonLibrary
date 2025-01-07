//
//    複数のエンコーダーのカウント値を取得する例 (Raspberry Pi Pico)
//
//    Copyright (c) 2022 udonrobo
//

#include <Udon.hpp>

static Udon::EncoderPico encoders[]{
    { 0, 1 },
    { 2, 3 },
    { 4, 5 },
    { 6, 7 },
    { 8, 9 },
    { 10, 11 },
    { 12, 13 },
    { 14, 15 },  // RP2040内蔵のステートマシンを用いるため最大8個まで計測可能
};

void setup()
{
    for (auto&& encoder : encoders)
    {
        encoder.begin();
    }
}

void loop()
{
    for (auto&& encoder : encoders)
    {
        Serial.print(encoder.read());
        Serial.print('\t');
    }
    Serial.println();
    delay(10);
}
