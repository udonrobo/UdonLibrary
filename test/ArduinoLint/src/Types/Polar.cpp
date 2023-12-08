#include <Arduino.h>

#include <Udon/Types/Polar.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr Udon::Polar a;

        constexpr Udon::Polar b = a;

        constexpr Udon::Polar c = { 100, 200 };

        (void)b;
        (void)c;
    }

    // コピー代入演算子
    {
        Udon::Polar a, b;
        a = b;
    }

    // 二項演算子
    {
        Udon::Polar a, b = { 100, 200 };
        a + b;
        a - b;
        a * b;
        a / b;
    }

    // 複合代入演算子
    {
        Udon::Polar a, b;
        a += b;
        a -= b;
        a *= b;
        a /= b;
    }

    // 比較演算子
    {
        constexpr Udon::Polar a = { 100, 200 };
        constexpr Udon::Polar b = { 100, 200 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::Polar a = { 0, 0 };
        static_assert((bool)a == false, "");
    }

    // その他関数
    {
        Udon::Polar a;
        a.x();
        a.y();
    }

    // シリアライズ
    {
        Udon::Polar a;
        static_assert(Udon::SerializedSize<Udon::Polar>() == 8 + 1, "");
        const auto b = Udon::Pack(a);
        (void)Udon::Unpack<Udon::Polar>(b);
    }
}