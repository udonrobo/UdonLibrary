#include <Arduino.h>

#include <udon/types/Vector2D.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::Vec2 a;

        constexpr udon::Vec2 b = a;

        constexpr udon::Vec2 c = { 100, 200 };
    }

    // コピー代入演算子
    {
        udon::Vec2 a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr udon::Vec2 a, b = { 10, 20 };
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
        udon::Vec2 a, b;
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
        constexpr udon::Vec2 a = { 100, 200 };
        constexpr udon::Vec2 b = { 100, 200 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::Vec2 a = { 0, 0 };

        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        udon::Vec2 a;
        a.clear();
        a.rotatedAt({}, 0);
        a.rotated(0);
        a.angle();
        a.angleAt({});
        a.distanceFrom({});
        a.length();
        a.show();
    }

    // シリアライズ
    {
        udon::Vec2 a;
        static_assert(udon::CapacityWithChecksum(a) == 8 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::Vec2>(b);
    }
}