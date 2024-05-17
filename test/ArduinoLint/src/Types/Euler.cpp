//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>

#include <Udon/Types/Euler.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr Udon::Euler a;

        constexpr Udon::Euler b = a;

        constexpr Udon::Euler c = { 100, 200, 300 };

        (void)b;
        (void)c;
    }

    // コピー代入演算子
    {
        Udon::Euler a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr Udon::Euler a, b = { 10, 20, 30 };
        a + b;
        a - b;
        a* b;
        a / b;
        a + 100;
        a - 100;
        a * 100;
        a / 100;
    }

    // 複合代入演算子
    {
        Udon::Euler a, b;
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
        constexpr Udon::Euler a = { 100, 200, 300 };
        constexpr Udon::Euler b = { 100, 200, 300 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::Euler a = { 0, 0, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        Udon::Euler a;
        a.normalized(-PI, PI);
        a.toQuaternion();
        a.directionRevision({ true, false, true });
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        Udon::Euler a;
        static_assert(Udon::SerializedSize<Udon::Euler>() == 12 + 1, "");
        const auto b = Udon::Serialize(a);
        (void)Udon::Deserialize<Udon::Euler>(b);
    }
}