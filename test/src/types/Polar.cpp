#include <Arduino.h>

#include <udon/types/Polar.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::Polar a;

        constexpr udon::Polar b = a;

        constexpr udon::Polar c = { 100, 200 };
    }

    // コピー代入演算子
    {
        udon::Polar a, b;
        a = b;
    }

    // 二項演算子
    {
        udon::Polar a, b = { 100, 200 };
        a + b;
        a - b;
        a * b;
        a / b;
    }

    // 複合代入演算子
    {
        udon::Polar a, b;
        a += b;
        a -= b;
        a *= b;
        a /= b;
    }

    // 比較演算子
    {
        constexpr udon::Polar a = { 100, 200 };
        constexpr udon::Polar b = { 100, 200 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::Polar a = { 0, 0 };
        static_assert((bool)a == false, "");
    }

    // その他関数
    {
        udon::Polar a;
        a.x();
        a.y();
    }

    // シリアライズ
    {
        udon::Polar a;
        static_assert(udon::CapacityWithChecksum(a) == 8 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::Polar>(b);
    }
}