//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>

#include <Udon/Types/RGB.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr Udon::RGB a;

        constexpr Udon::RGB b = a;

        constexpr Udon::RGB c = { 100, 200, 255 };

        (void)b;
        (void)c;
    }

    // 変換コンストラクタ
    {
        Udon::RGB a = Udon::HSV{ 100, 200, 255 };

        (void)a;
    }

    // コピー代入演算子
    {
        Udon::RGB a, b;
        a = b;
    }

    // 比較演算子
    {
        constexpr Udon::RGB a = { 100, 200, 255 };
        constexpr Udon::RGB b = { 100, 200, 255 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::RGB a = { 0, 0, 0 };

        static_assert((bool)a == false, "");
    }

    // その他関数
    {
        Udon::RGB a;
        a.show();
        a.toHSV();
    }

    // シリアライズ
    {
        Udon::RGB a;
        static_assert(Udon::SerializedSize<Udon::RGB>() == 3 + 1, "");
        const auto b = Udon::Serialize(a);
        (void)Udon::Deserialize<Udon::RGB>(b);
    }
}