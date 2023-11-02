#include <Arduino.h>

#include <Udon/Com/Serialization.hpp>
#include <Udon/Types/Float.hpp>


inline void test()
{
    // シリアライズ
    {
        Udon::float32_t a;
        static_assert(Udon::PackedSize<Udon::float32_t>() == 4 + 1, "");
        const auto b = Udon::Pack(a);
        (void)Udon::Unpack<Udon::float32_t>(b);
    }
}