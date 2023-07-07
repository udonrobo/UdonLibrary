#include <Udon/Utility/Parsable.hpp>

struct Parsable
{
    int a;
    int b;
    int c;
    UDON_PARSABLE(a, b, c);
};
struct UnParsable
{
    int a;
    int b;
    int c;
};

inline void test()
{
    // パース可能
    static_assert(Udon::IsParsable<int>::value, "");               // プリミティブ型
    static_assert(Udon::IsParsable<int[]>::value, "");             // プリミティブ型配列
    static_assert(Udon::IsParsable<int[2][2]>::value, "");         // プリミティブ型多次元配列
    static_assert(Udon::IsParsable<float>::value, "");             // プリミティブ型
    static_assert(Udon::IsParsable<float[]>::value, "");           // プリミティブ型配列
    static_assert(Udon::IsParsable<float[2][2]>::value, "");       // プリミティブ型多次元配列
    static_assert(Udon::IsParsable<Parsable>::value, "");          // ユーザー定義型
    static_assert(Udon::IsParsable<Parsable[]>::value, "");        // ユーザー定義型配列
    static_assert(Udon::IsParsable<Parsable[2][2]>::value, "");    // ユーザー定義型多次元配列

    // パース不可
    static_assert(not Udon::IsParsable<int*>::value, "");          // プリミティブ型ポインタ
    static_assert(not Udon::IsParsable<int*[]>::value, "");        // プリミティブ型ポインタ配列
    static_assert(not Udon::IsParsable<float*>::value, "");        // プリミティブ型ポインタ
    static_assert(not Udon::IsParsable<float*[]>::value, "");      // プリミティブ型ポインタ配列
    static_assert(not Udon::IsParsable<int&>::value, "");          // プリミティブ型参照
    static_assert(not Udon::IsParsable<float&>::value, "");        // プリミティブ型参照
    static_assert(not Udon::IsParsable<UnParsable>::value, "");    // パース不可能なユーザー定義型
}