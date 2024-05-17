//
//    StringView Unit Test
//
//    Copyright (c) 2022-2024 udonrobo
//


#include <gtest/gtest.h>
#include <Udon/Types/StringView.hpp>
#include <sstream>


TEST(StringView, Construct)
{
    const auto cstring = "test string";

    {
        // デフォルトコンストラクタ
        Udon::StringView sv;
        EXPECT_TRUE(sv.size() == 0);
        EXPECT_TRUE(sv.empty());
        EXPECT_TRUE(sv.data() == nullptr);
        EXPECT_TRUE(sv == "");
    }

    {
        // C言語スタイル文字列から構築
        Udon::StringView sv{ cstring };
        EXPECT_TRUE(sv.size() == strlen(cstring));
        EXPECT_TRUE(sv.data() == cstring);
        EXPECT_TRUE(sv == cstring);
    }

    {
        // 文字列先頭を指すポインタ、文字数から構築
        Udon::StringView sv{ cstring, strlen(cstring) };
        EXPECT_TRUE(sv.size() == strlen(cstring));
        EXPECT_TRUE(sv.data() == cstring);
        EXPECT_TRUE(sv == cstring);
    }

    {
        // 文字列イテレータから構築
        Udon::StringView sv{ cstring, cstring + strlen(cstring) };
        EXPECT_TRUE(sv.size() == strlen(cstring));
        EXPECT_TRUE(sv.data() == cstring);
        EXPECT_TRUE(sv == cstring);
    }

    {
        // リテラルから構築
        using namespace Udon::Literals;
        constexpr Udon::StringView sv = "test string"_sv;
        static_assert(sv.size() == 11);
    }

    {
        // std::stringから構築
        std::string      str = "test string";
        Udon::StringView sv{ str };
        EXPECT_TRUE(sv.size() == str.size());
        EXPECT_TRUE(sv == str);
    }
}


TEST(StringView, CompareOperator)
{
    Udon::StringView sv = "test string";

    {
        // operator==
        EXPECT_TRUE(sv == "test string");
        EXPECT_TRUE(not(sv == "test"));
    }

    {
        // operator!=
        EXPECT_TRUE(sv != "test str");
        EXPECT_TRUE(not(sv != "test string"));
    }

    {
        // 比較対象の方が長い場合
        EXPECT_TRUE(sv != "test string string");
    }
}


TEST(StringView, Substring)
{
    Udon::StringView sv = "test string";

    {
        auto sub = sv.substring(0, 4);
        EXPECT_TRUE(sub == "test");
    }

    {
        auto sub = sv.substring(5);
        EXPECT_TRUE(sub == "string");
    }

    {
        // 範囲外指定 (開始位置が範囲外の場合、空ビューを返す)
        auto sub = sv.substring(100);
        EXPECT_TRUE(sub == "");
    }

    {
        // 範囲指定異常 (開始位置より終端位置の方が大きい場合、ビューを返す)
        auto sub = sv.substring(5, 3);
        EXPECT_TRUE(sub == "");
    }

    {
        // 範囲外指定 (終端位置の範囲外は自動で調整)
        auto sub = sv.substring(0, 100);
        EXPECT_TRUE(sub == "test string");
    }
}


TEST(StringView, RemovePrefix)
{
    Udon::StringView sv = "test string";

    {
        auto sub = sv.removePrefix(strlen("test "));
        EXPECT_TRUE(sub == "string");
    }

    {
        // 範囲外指定 (削除個数が文字数を上回る場合、空ビューを返す)
        auto sub = sv.removePrefix(100);
        EXPECT_TRUE(sub == "");
    }
}


TEST(StringView, RemoveSuffix)
{
    Udon::StringView sv = "test string";

    {
        auto sub = sv.removeSuffix(strlen(" string"));
        EXPECT_TRUE(sub == "test");
    }

    {
        // 範囲外指定 (削除個数が文字数を上回る場合、空ビューを返す)
        auto sub = sv.removeSuffix(100);
        EXPECT_TRUE(sub == "");
    }
}


TEST(StringView, StartsWith)
{
    Udon::StringView sv = "test string";

    // 正常系
    EXPECT_TRUE(sv.startsWith("test"));
    EXPECT_TRUE(sv.startsWith(""));
    EXPECT_TRUE(sv.startsWith("test string"));

    // 異常系
    EXPECT_TRUE(not sv.startsWith("string"));
    EXPECT_TRUE(not sv.startsWith("test string string"));
    EXPECT_TRUE(not sv.startsWith("test string "));
    EXPECT_TRUE(not sv.startsWith("string"));
}


TEST(StringView, EndsWith)
{
    Udon::StringView sv = "test string";

    // 正常系
    EXPECT_TRUE(sv.endsWith("string"));
    EXPECT_TRUE(sv.endsWith(""));
    EXPECT_TRUE(sv.endsWith("test string"));

    // 異常系
    EXPECT_TRUE(not sv.endsWith("test"));
    EXPECT_TRUE(not sv.endsWith("test string string"));
    EXPECT_TRUE(not sv.endsWith(" test string"));
    EXPECT_TRUE(not sv.endsWith("test"));
}


TEST(StringView, Split)
{
    {
        Udon::StringView sv = "This is a test string";

        std::vector<Udon::StringView> result = sv.split(' ');

        EXPECT_TRUE(result.size() == 5);
        EXPECT_TRUE(result.at(0) == "This");
        EXPECT_TRUE(result.at(1) == "is");
        EXPECT_TRUE(result.at(2) == "a");
        EXPECT_TRUE(result.at(3) == "test");
        EXPECT_TRUE(result.at(4) == "string");
    }

    {
        Udon::StringView sv = "";

        std::vector<Udon::StringView> result = sv.split(' ');

        EXPECT_TRUE(result.size() == 0);
    }

    {
        Udon::StringView sv = "This is a test string";

        std::vector<Udon::StringView> result = sv.split('*');

        EXPECT_TRUE(result.size() == 1);
        EXPECT_TRUE(result.at(0) == "This is a test string");
    }
}


TEST(StringView, ToNumber)
{

    // 正常系

    {
        Udon::StringView    sv  = "123";
        Udon::Optional<int> num = sv.toNumber<int>();
        EXPECT_TRUE(num);
        EXPECT_TRUE(*num == 123);
    }

    {
        Udon::StringView sv  = "123";
        auto             num = sv.toNumber<long>();
        EXPECT_TRUE(num);
        EXPECT_TRUE(*num == 123l);
    }

    {
        Udon::StringView sv  = "123.456";
        auto             num = sv.toNumber<float>();
        EXPECT_TRUE(num);
        EXPECT_TRUE(*num == 123.456f);
    }

    {
        Udon::StringView sv  = "123.456";
        auto             num = sv.toNumber<double>();
        EXPECT_TRUE(num);
        EXPECT_TRUE(*num == 123.456);
    }

    // 異常系

    {
        Udon::StringView sv  = "123.456";
        auto             num = sv.toNumber<int>();
        EXPECT_TRUE(not num);
    }

    {
        Udon::StringView sv  = "string";
        auto             num = sv.toNumber<int>();
        EXPECT_TRUE(not num);
    }
}

TEST(StringView, ToString)
{
    Udon::StringView sv  = "test string";
    std::string      str = sv.toString();
    EXPECT_TRUE(str == "test string");
}

TEST(StringView, OutputStream)
{
    Udon::StringView   sv = "test string";
    std::ostringstream oss;
    oss << sv;
    EXPECT_TRUE(oss.str() == "test string");
}

TEST(StringView, Iterator)
{
    Udon::StringView string = Udon::StringView{ "-test string=" }.removePrefix(1).removeSuffix(1);

    // forward iterator test
    //
    // -"test string"=
    //   ^begin      ^end
    //   ^cbegin     ^cend
    //
    EXPECT_TRUE(*string.begin() == 't');
    EXPECT_TRUE(*string.cbegin() == 't');
    EXPECT_TRUE(*string.end() == '=');
    EXPECT_TRUE(*string.cend() == '=');

    // reverse iterator test
    //
    // -"test string"=
    // ^rend       ^rbegin
    // ^crend      ^crbegin
    //
    EXPECT_TRUE(*string.rbegin() == 'g');
    EXPECT_TRUE(*string.crbegin() == 'g');
    EXPECT_TRUE(*string.rend() == '-');
    EXPECT_TRUE(*string.crend() == '-');
}