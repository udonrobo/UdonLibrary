#include <Arduino.h>

#include <udon/types/Quaternion.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::Quaternion a;

        constexpr udon::Quaternion b = a;

        constexpr udon::Quaternion c = { 100, 200, 300, 400 };
    }

    // コピー代入演算子
    {
        udon::Quaternion a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr udon::Quaternion a, b = { 10, 20, 30, 40 };
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
        udon::Quaternion a, b;
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
        constexpr udon::Quaternion a = { 100, 200, 300, 40 };
        constexpr udon::Quaternion b = { 100, 200, 300, 40 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::Quaternion a = { 0, 0, 0, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        udon::Quaternion a;
        a.toEuler();
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        udon::Quaternion a;
        static_assert(udon::CapacityWithChecksum(a) == 16 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::Quaternion>(b);
    }
}