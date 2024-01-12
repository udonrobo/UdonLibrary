//
//    LoRa (E220) 送信用ファームウエア
//    Copyright (c) 2022-2023 Okawa Yusuke
//

#include <Udon.hpp>

Udon::E220 lora{ Serial1,      2,       3,       4 };
//               ^--UARTPort   ^--M0Pin ^--M1Pin ^--AUXPin

Udon::LoRaReader<int> reader{ lora };
//               ^--Message   ^--LoRa

void setup() 
{
    // 送信を行うには、同じ周波数帯(チャンネル)で、かつ送信先のアドレスを指定している必要がある
    constexpr uint8_t channel = 0x0a;
    constexpr uint16_t myAddress = 0x0002;
    constexpr uint16_t tarAddress = 0x0001;
    lora.begin(channel, myAddress, tarAddress);
}

void loop()
{
    // LoRaデバイスを更新(受信処理を行う)
    {
        lora.update();
    }

    // 受信データを出力
    if (const auto message = reader.getMessage())
    {
        // 受信成功
        Serial.println(*message);
    }
    else
    {
        // 受信失敗
        Serial.println("receive error!");
    }
    
    delay(10);
}
