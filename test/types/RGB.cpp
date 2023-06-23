#include <udon/types/RGB.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr udon::RGB a;

        constexpr udon::RGB b = a;

        constexpr udon::RGB c = { 100, 200, 255 };
    }

    // 変換コンストラクタ
    {
        udon::RGB a = udon::HSV{ 100, 200, 255 };
    }

    // コピー代入演算子
    {
        udon::RGB a, b;
        a = b;
    }

    // 比較演算子
    {
        constexpr udon::RGB a = { 100, 200, 255 };
        constexpr udon::RGB b = { 100, 200, 255 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::RGB a = { 0, 0, 0 };

        static_assert((bool)a == false, "");
    }

    // その他関数
    {
        udon::RGB a;
        a.show();
        a.toHSV();
    }

    // シリアライズ
    {
        udon::RGB a;
        static_assert(udon::CapacityWithChecksum(a) == 3 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::RGB>(b);
    }
}