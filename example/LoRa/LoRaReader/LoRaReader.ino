//
//    LoRa (E220) 受信用ファームウエア
//

#include <Udon.hpp>

static Udon::E220Reader<uint64_t> lora({
    .serial = Serial1,
    .m0 = 2,
    .m1 = 3, 
    .aux = 4,
    .channel = 0
});

void setup()
{
    lora.begin();
}

void loop()
{
    if (const auto message = lora.getMessage())
    {
        Serial.println(*message);
    }
    delay(1);
}