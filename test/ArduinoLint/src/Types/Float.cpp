#include <Arduino.h>

#include <Udon/Com/Serialization.hpp>
#include <Udon/Types/Float.hpp>


inline void test()
{
    // シリアライズ
    {
        Udon::float32_t a;
        static_assert(Udon::SerializedSize<Udon::float32_t>() == 4 + 1, "");
        const auto b = Udon::Serialize(a);
        (void)Udon::Deserialize<Udon::float32_t>(b);
    }
}