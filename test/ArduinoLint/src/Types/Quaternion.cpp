#include <Arduino.h>

#include <Udon/Types/Quaternion.hpp>

inline void test()
{

    // コンストラクタ
    {
        constexpr Udon::Quaternion a;

        constexpr Udon::Quaternion b = a;

        constexpr Udon::Quaternion c = { 100, 200, 300, 400 };

        (void)b;
        (void)c;
    }

    // コピー代入演算子
    {
        Udon::Quaternion a, b;
        a = b;
    }

    // 二項演算子
    {
        constexpr Udon::Quaternion a, b = { 10, 20, 30, 40 };
        a * b;
    }

    // 複合代入演算子
    {
        Udon::Quaternion a, b;
        a *= b;
    }

    // 比較演算子
    {
        constexpr Udon::Quaternion a = { 100, 200, 300, 40 };
        constexpr Udon::Quaternion b = { 100, 200, 300, 40 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr Udon::Quaternion a = { 0, 0, 0, 0 };
        static_assert((bool)a == false, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        Udon::Quaternion a;
        a.inverce();
        a.toPitch();
        a.toRoll();
        a.toYaw();
        a.toEuler();
        a.clear();
        a.show();
    }

    // シリアライズ
    {
        Udon::Quaternion a;
        static_assert(Udon::CapacityWithChecksum<Udon::Quaternion>() == 16 + 1, "");
        const auto b = Udon::Pack(a);
        (void)Udon::Unpack<Udon::Quaternion>(b);
    }
}