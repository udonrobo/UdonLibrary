#include <Arduino.h>

#include <UdonFwd.hpp>
#include <udon/types/Vector4D.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::Vec4 a;

        constexpr udon::Vec4 b = a;

        constexpr udon::Vec4 c = { 100, 200, 300, 400 };
    }

    // コピー代入演算子
    {
        udon::Vec4 a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr udon::Vec4 a, b = { 10, 20, 30, 40 };
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
        udon::Vec4 a, b;
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
        constexpr udon::Vec4 a = { 100, 200, 300, 40 };
        constexpr udon::Vec4 b = { 100, 200, 300, 40 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::Vec4 a = { 0, 0, 0, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        udon::Vec4 a;
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        udon::Vec4 a;
        static_assert(udon::CapacityWithChecksum(a) == 16 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::Vec4>(b);
    }
}