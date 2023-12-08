#include <Arduino.h>

#include <Udon/Types/Vector3D.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr Udon::Vec3 a;

        constexpr Udon::Vec3 b = a;

        constexpr Udon::Vec3 c = { 100, 200, 300 };

        (void)b;
        (void)c;
    }

    // コピー代入演算子
    {
        Udon::Vec3 a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr Udon::Vec3 a, b = { 10, 20, 30 };
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
        Udon::Vec3 a, b;
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
        constexpr Udon::Vec3 a = { 100, 200, 300 };
        constexpr Udon::Vec3 b = { 100, 200, 300 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::Vec3 a = { 0, 0, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        Udon::Vec3 a;
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        Udon::Vec3 a;
        static_assert(Udon::SerializedSize<Udon::Vec3>() == 12 + 1, "");
        const auto b = Udon::Serialize(a);
        (void)Udon::Deserialize<Udon::Vec3>(b);
    }
}