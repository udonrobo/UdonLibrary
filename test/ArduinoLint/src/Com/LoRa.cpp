#if __cplusplus >= 201402L

#    include <Arduino.h>
#    include <Udon/Com/LoRa.hpp>

inline void ReaderTest()
{
    Udon::E220Reader<uint64_t> lora({
        .serial  = Serial1,
        .m0      = 2,
        .m1      = 3,
        .aux     = 4,
        .channel = 0,
        .address = 0x0000,
    });

    lora.begin();

    auto message = lora.getMessage();

    static_assert(std::is_same<decltype(message), Udon::Optional<uint64_t>>::value, "Invalid type");

    (void)lora.getRssi();
}

inline void WriterTest()
{
    Udon::E220Writer<uint64_t> lora({
        .serial  = Serial1,
        .m0      = 2,
        .m1      = 3,
        .aux     = 4,
        .channel = 0,
        .address = 0x0000,
    });

    lora.begin();
    lora.setMessage(1234567890);
}

#endif