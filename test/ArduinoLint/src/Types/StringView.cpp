#include <Arduino.h>
#include <Udon/Types/StringView.hpp>

using Udon::StringView;

inline void testConstructor()
{
    StringView defaultConstructor{};

    StringView stringLiteral{ "Hello, World!" };

    StringView string{ "Hello, World!", 13 };

    StringView stringPointer{ stringLiteral.data(), 13 };
}

inline void testMethod()
{
    using namespace Udon::Literals;

    constexpr auto stringView = "100 200.00 300"_sv;

    static_assert((bool)stringView, "");
    constexpr auto data = stringView.data();
    (void)data;
    static_assert(stringView.size() == 14, "");
    static_assert(not stringView.empty(), "");
    static_assert(stringView[0] == '1', "");
    static_assert(stringView.front() == '1', "");
    static_assert(stringView.back() == '0', "");

    (void)stringView.at(0);
    (void)stringView.substring(0, 5);
    (void)stringView.substring(7);
    (void)stringView.substringUntil(' ');
    (void)stringView.substringFrom(' ');
    (void)stringView.startsWith("100");
    (void)stringView.endsWith("300");
    (void)stringView.split(' ');
    (void)stringView.toNumber<int>();
    (void)stringView.toNumber<float>();
    (void)stringView.toNumber<double>();
    (void)stringView.toString();

    auto stringView1 = "100 200.00 300"_sv;
    auto stringView2 = "100 200.00 300"_sv;
    stringView1.swap(stringView2);

    "abc"_sv == "abc"_sv;
    "abc"_sv != "def"_sv;

}