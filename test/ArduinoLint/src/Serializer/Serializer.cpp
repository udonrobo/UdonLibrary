//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Traits/EnumerableMacro.hpp>

struct Hoge
{
    int a;
    int b;
    int c;
    UDON_ENUMERABLE(a, b, c);
};

inline void test()
{
    Hoge hoge { 1, 2, 3 };

    const auto packed = Udon::Serialize(hoge);

    const auto unpacked = Udon::Deserialize<Hoge>(packed);

    (void)unpacked;
}