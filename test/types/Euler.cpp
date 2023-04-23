#include <Arduino.h>

#include <udon/types/Euler.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::Euler a;

        constexpr udon::Euler b = a;

        constexpr udon::Euler c = { 100, 200, 300 };
    }

    // コピー代入演算子
    {
        udon::Euler a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr udon::Euler a, b = { 10, 20, 30 };
        a + b;
        a - b;
        a * b;
        a / b;
        a + 100;
        a - 100;
        a * 100;
        a / 100;
    }

    // 複合代入演算子
    {
        udon::Euler a, b;
        a += b;
        a -= b;
        a *= b;
        a /= b;
        a += 100;
        a -= 100;
        a *= 100;
        a /= 100;
    }

    // 比較演算子
    {
        constexpr udon::Euler a = { 100, 200, 300 };
        constexpr udon::Euler b = { 100, 200, 300 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::Euler a = { 0, 0, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        udon::Euler a;
		a.normalized(-PI, PI);
		a.directionRevision({ true, false, true });
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        udon::Euler a;
        static_assert(udon::CapacityWithChecksum(a) == 12 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::Euler>(b);
    }
}