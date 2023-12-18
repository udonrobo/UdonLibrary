#include <Arduino.h>

#include <Udon/Types/Position.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr Udon::Pos a;

        constexpr Udon::Pos b = a;

        constexpr Udon::Pos c = { { 100, 200 }, 300 };

        (void)b;
        (void)c;
    }

    // コピー代入演算子
    {
        Udon::Pos a, b;
        a = b;
    }

    // 二項演算子
    {
        Udon::Pos a, b = { { 100, 200 }, 300 };
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
        Udon::Pos a, b;
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
        constexpr Udon::Pos a = { { 100, 200 }, 300 };
        constexpr Udon::Pos b = { { 100, 200 }, 300 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::Pos a = { { 0, 0 }, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        Udon::Pos a;
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        Udon::Pos a;
        static_assert(Udon::SerializedSize<Udon::Pos>() == 12 + 1, "");
        const auto b = Udon::Serialize(a);
        (void)Udon::Deserialize<Udon::Pos>(b);
    }
}