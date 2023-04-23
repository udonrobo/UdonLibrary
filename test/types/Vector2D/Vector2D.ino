#include <UdonFwd.hpp>
#include <udon/types/Vector2D.hpp>

void setup()
{

//	std::vector<int> a;
//	std::reverse(a.begin(), a.end());

//	Serial.print('\t');

    // コンストラクタ
    {
        constexpr udon::Vector2D<double> a;

        constexpr udon::Vector2D<double> b = a;

        constexpr udon::Vector2D<double> c = { 100, 200 };
    }

    // コピー代入演算子
    {
        udon::Vector2D<double> a, b;
        a = b;
    }

    // 算術演算子
    {
        udon::Vector2D<double> a, b;
        a + b;
        a - b;
        a* b;
        a / b;
        a + 100;
        a - 100;
        a * 100;
        a / 100;

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
        constexpr udon::Vector2D<double> a = { 100, 200 };
        constexpr udon::Vector2D<double> b = { 100, 200 };

        static_assert(a == b, "");
    }

    // operator bool
    {
        constexpr udon::Vector2D<double> a = { 0, 0 };

        static_assert(!a, "");
        static_assert(a.isZero(), "");
    }

    // その他関数
    {
        udon::Vector2D<double> a;

        a.clear();
        a.rotatedAt({}, 0);
        a.rotated(0);
        a.angle();
        a.angleAt({});
        a.distanceFrom({});
        a.length();
        a.show();
    }
}

void loop()
{
}
