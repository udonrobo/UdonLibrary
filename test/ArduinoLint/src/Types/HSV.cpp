//
//    構文テスト
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>

#include <Udon/Types/HSV.hpp>

__attribute__((unused)) static void test()
{

    // コンストラクタ
    {
        constexpr Udon::HSV a;

        constexpr Udon::HSV b = a;

        constexpr Udon::HSV c = { 100, 200, 255 };

        (void)b;
        (void)c;
    }

    // 変換コンストラクタ
    {
        Udon::HSV a = Udon::RGB{ 100, 200, 255 };

        (void)a;
    }

    // コピー代入演算子
    {
        Udon::HSV a, b;
        a = b;
    }

    // 比較演算子
    {
        constexpr Udon::HSV a = { 100, 200, 255 };
        constexpr Udon::HSV b = { 100, 200, 255 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::HSV a = { 0, 0, 0 };

        static_assert((bool)a == false, "");
    }

    // その他関数
    {
        Udon::HSV a;
        a.show();
        a.toRGB();
    }

    // シリアライズ
    {
        Udon::HSV a;
        static_assert(Udon::SerializedSize<Udon::HSV>() == 3 + 1, "");
        const auto b = Udon::Serialize(a);
        (void)Udon::Deserialize<Udon::HSV>(b);
    }
}