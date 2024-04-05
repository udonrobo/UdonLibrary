#if __cplusplus >= 201402L

#    include <Arduino.h>
#    include <Udon/Com/LoRa.hpp>

inline void test()
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
}

#endif