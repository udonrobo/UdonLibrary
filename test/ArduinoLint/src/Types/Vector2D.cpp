//
//    構文テスト
//
//    Copyright (c) 2022 udonrobo
//

#include <Arduino.h>

#include <Udon/Types/Vector2D.hpp>
#include <Udon/Serializer/Serializer.hpp>

__attribute__((unused)) static void test()
{

    // コンストラクタ
    {
        constexpr Udon::Vec2 a;

        constexpr Udon::Vec2 b = a;

        constexpr Udon::Vec2 c = { 100, 200 };

        (void)b;
        (void)c;
    }

    // コピー代入演算子
    {
        Udon::Vec2 a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr Udon::Vec2 a, b = { 10, 20 };
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
        Udon::Vec2 a, b;
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
        constexpr Udon::Vec2 a = { 100, 200 };
        constexpr Udon::Vec2 b = { 100, 200 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::Vec2 a = { 0, 0 };

        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        Udon::Vec2 a;
        a.clear();
        a.rotatedAt({}, 0);
        a.rotated(0);
        a.angle();
        a.angleAt({});
        a.distanceFrom({});
        a.length();
        a.lengthSq();
        a.scaledLength(0);
        a.scaleLength(0);
        a.normalized();
        a.show();
    }

    // シリアライズ
    {
        Udon::Vec2 a;
        static_assert(Udon::SerializedSize<Udon::Vec2>() == 8 + 1, "");
        const auto b = Udon::Serialize(a);
        (void)Udon::Deserialize<Udon::Vec2>(b);
    }
}