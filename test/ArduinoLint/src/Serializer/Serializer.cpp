#include <Arduino.h>

#include <Udon/Com/Serialization.hpp>
#include <Udon/Utility/Parsable.hpp>

struct Hoge
{
    int a;
    int b;
    int c;
    UDON_PARSABLE(a, b, c);
};

inline void test()
{
    Hoge hoge { 1, 2, 3 };

    const auto packed = Udon::Pack(hoge);

    const auto unpacked = Udon::Unpack<Hoge>(packed);

    // assert(hoge == unpacked);
}