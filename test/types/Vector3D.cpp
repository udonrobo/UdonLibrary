#include <Arduino.h>

#include <UdonFwd.hpp>
#include <udon/types/Vector3D.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::Vec3 a;

        constexpr udon::Vec3 b = a;

        constexpr udon::Vec3 c = { 100, 200, 300 };
    }

    // コピー代入演算子
    {
        udon::Vec3 a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr udon::Vec3 a, b = { 10, 20, 30 };
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
        udon::Vec3 a, b;
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
        constexpr udon::Vec3 a = { 100, 200, 300 };
        constexpr udon::Vec3 b = { 100, 200, 300 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::Vec3 a = { 0, 0, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        udon::Vec3 a;
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        udon::Vec3 a;
        static_assert(udon::CapacityWithChecksum(a) == 12 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::Vec3>(b);
    }
}