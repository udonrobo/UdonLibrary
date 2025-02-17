//
//    構文テスト
//
//    Copyright (c) 2022 udonrobo
//

#include <Arduino.h>

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Types/Float.hpp>

__attribute__((unused)) static void test()
{
    // シリアライズ
    {
        Udon::Float32 a;
        static_assert(Udon::SerializedSize<Udon::Float32>() == 4 + 1, "");
        const auto b = Udon::Serialize(a);
        (void)Udon::Deserialize<Udon::Float32>(b);
    }
}