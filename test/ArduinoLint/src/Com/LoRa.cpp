//
//    構文テスト
//
//    Copyright (c) 2022 udonrobo
//


#include <Arduino.h>
#include <Udon/Com/LoRa.hpp>

__attribute__((unused)) static void ReaderTest()
{

#if __cplusplus >= 201402L
    Udon::E220Reader<uint64_t> lora({
        .serial = Serial1,
        .m0 = 2,
        .m1 = 3,
        .aux = 4,
        .channel = 0,
        .address = 0x0000,
    });
#else
    Udon::E220Reader<uint64_t> lora({
        Serial,
        2,
        3,
        4,
        0,
        0x0000,
    });
#endif


    lora.begin();
    lora.begin(0x00);

    auto message = lora.getMessage();

    static_assert(std::is_same<decltype(message), Udon::Optional<uint64_t>>::value, "Invalid type");

    (void)lora.getRssi();
}

__attribute__((unused)) static void WriterTest()
{
#if __cplusplus >= 201402L
    Udon::E220Writer<uint64_t> lora({
        .serial = Serial1,
        .m0 = 2,
        .m1 = 3,
        .aux = 4,
        .channel = 0,
        .address = 0x0000,
    });
#else
    Udon::E220Writer<uint64_t> lora({
        Serial,
        2,
        3,
        4,
        0,
        0x0000,
    });
#endif

    lora.begin();
    lora.begin(0x00);

    lora.setMessage(1234567890);
}
