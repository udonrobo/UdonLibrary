//
//    LoRa (E220) 送信用ファームウエア
//

#include <Udon.hpp>

Udon::E220 lora{ Serial1,      10,      11,       2 };
//               ^--UARTPort   ^--M0Pin ^--M1Pin ^--AUXPin

Udon::LoRaWriter<int> writer{ lora };
//               ^--Message   ^--LoRa

void setup() 
{
    // 実験基板の TX/RX ピンがデフォルトピンでなかったので変更
    Serial1.setTX(12);
    Serial1.setRX(13);

    // 送信を行うには、同じ周波数帯(チャンネル)で、かつ送信先のアドレスを指定している必要がある
    constexpr uint8_t channel = 0x0a;
    constexpr uint16_t myAddress = 0x0001;
    constexpr uint16_t tarAddress = 0x0002;
    lora.begin(channel, myAddress, tarAddress);
}

void loop() {

    // 送信データをセット
    {
        const int sendData = static_cast<int>(millis());
        writer.setMessage(sendData);
    }

    // LoRaデバイスを更新(送信処理を行う)
    {
        lora.update();
    }
    
    delay(10);
}
