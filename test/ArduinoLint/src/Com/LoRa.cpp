#include <Arduino.h>
#include <Udon/Com/LoRa.hpp>

inline void test()
{
    Udon::E220Reader<uint64_t> lora(Udon::E220Base::Config{

#if defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560)
        // .serial = Serial,
#else
        .serial = Serial1,
#endif

        .m0      = 2,
        .m1      = 3,
        .aux     = 4,
        .channel = 0
    });

    lora.begin();
    // const auto message = lora.getMessage();

    // static_assert(std::is_same<decltype(message), std::optional<uint64_t>>::value, "Invalid type");
}