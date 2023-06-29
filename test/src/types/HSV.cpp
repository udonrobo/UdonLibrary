#include <Arduino.h>

#include <udon/types/HSV.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::HSV a;

        constexpr udon::HSV b = a;

        constexpr udon::HSV c = { 100, 200, 255 };
    }

    // 変換コンストラクタ
    {
        udon::HSV a = udon::RGB{ 100, 200, 255 };
    }

    // コピー代入演算子
    {
        udon::HSV a, b;
        a = b;
    }

    // 比較演算子
    {
        constexpr udon::HSV a = { 100, 200, 255 };
        constexpr udon::HSV b = { 100, 200, 255 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::HSV a = { 0, 0, 0 };

        static_assert((bool)a == false, "");
    }

    // その他関数
    {
        udon::HSV a;
        a.show();
        a.toRGB();
    }

    // シリアライズ
    {
        udon::HSV a;
        static_assert(udon::CapacityWithChecksum(a) == 3 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::HSV>(b);
    }
}