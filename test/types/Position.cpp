#include <Arduino.h>

#include <udon/types/Position.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::Pos a;

        constexpr udon::Pos b = a;

        constexpr udon::Pos c = { { 100, 200 }, 300 };
    }

    // コピー代入演算子
    {
        udon::Pos a, b;
        a = b;
    }

    // 二項演算子
    {
        udon::Pos a, b = { { 100, 200 }, 300 };
        a + b;
        a - b;
        a * b;
        a / b;
        a + 100.;
        a - 100.;
        a * 100.;
        a / 100.;
    }

    // 複合代入演算子
    {
        udon::Pos a, b;
        a += b;
        a -= b;
        a *= b;
        a /= b;
        a += 100.;
        a -= 100.;
        a *= 100.;
        a /= 100.;
    }

    // 比較演算子
    {
        constexpr udon::Pos a = { { 100, 200 }, 300 };
        constexpr udon::Pos b = { { 100, 200 }, 300 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::Pos a = { { 0, 0 }, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        udon::Pos a;
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        udon::Pos a;
        static_assert(udon::CapacityWithChecksum(a) == 12 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::Pos>(b);
    }
}