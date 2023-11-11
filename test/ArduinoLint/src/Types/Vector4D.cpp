#include <Arduino.h>

#include <Udon/Types/Vector4D.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr Udon::Vec4 a;

        constexpr Udon::Vec4 b = a;

        constexpr Udon::Vec4 c = { 100, 200, 300, 400 };

        (void)b;
        (void)c;
    }

    // コピー代入演算子
    {
        Udon::Vec4 a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr Udon::Vec4 a, b = { 10, 20, 30, 40 };
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
        Udon::Vec4 a, b;
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
        constexpr Udon::Vec4 a = { 100, 200, 300, 40 };
        constexpr Udon::Vec4 b = { 100, 200, 300, 40 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::Vec4 a = { 0, 0, 0, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        Udon::Vec4 a;
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        Udon::Vec4 a;
        static_assert(Udon::PackedSize<Udon::Vec4>() == 16 + 1, "");
        const auto b = Udon::Pack(a);
        (void)Udon::Unpack<Udon::Vec4>(b);
    }
}