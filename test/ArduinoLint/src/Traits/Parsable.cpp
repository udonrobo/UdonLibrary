#include <Udon/Traits/ParsableMacro.hpp>

/// @brief 列挙型
enum class Enum
{
    A,
    B,
    C,
};

/// @brief パース不可能なユーザー定義型
struct UnParsable
{
    int a;
    int b;
    int c;
};

/// @brief  パース可能なユーザー定義型
struct Parsable
{
    int a;
    int b;
    int c;
    UDON_PARSABLE(a, b, c);
};

/// @brief  内部にバース可能なメンバ変数を持つユーザー定義型
struct InnerParsable
{
    Parsable a;
    Parsable b;
    Parsable c;
    UDON_PARSABLE(a, b, c);
};

/// @brief 内部にパース不可能なメンバ変数を持つユーザー定義型
struct InnerUnParsable
{
    UnParsable a;
    UnParsable b;
    UnParsable c;
};

inline void test()
{
    // パース可能
    static_assert(Udon::Traits::Parsable<int>::value, "");                    // プリミティブ型
    static_assert(Udon::Traits::Parsable<int[2][2]>::value, "");              // プリミティブ型多次元配列
    static_assert(Udon::Traits::Parsable<float>::value, "");                  // プリミティブ型
    static_assert(Udon::Traits::Parsable<float[2][2]>::value, "");            // プリミティブ型多次元配列
    static_assert(Udon::Traits::Parsable<Enum>::value, "");                   // 列挙型
    static_assert(Udon::Traits::Parsable<Enum[2][2]>::value, "");             // 列挙型多次元配列
    static_assert(Udon::Traits::Parsable<Parsable>::value, "");               // ユーザー定義型
    static_assert(Udon::Traits::Parsable<Parsable[2][2]>::value, "");         // ユーザー定義型多次元配列
    static_assert(Udon::Traits::Parsable<InnerParsable>::value, "");          // 内部にパース可能なメンバ変数を持つ構造体
    static_assert(Udon::Traits::Parsable<InnerParsable[2][2]>::value, "");    // 内部にパース可能なメンバ変数を持つ構造体多次元配列

    // 空配列はパース不可
    static_assert(not Udon::Traits::Parsable<int[]>::value, "");         // プリミティブ型配列
    static_assert(not Udon::Traits::Parsable<float[]>::value, "");       // プリミティブ型配列
    static_assert(not Udon::Traits::Parsable<Parsable[]>::value, "");    // ユーザー定義型配列

    // パース不可
    static_assert(not Udon::Traits::Parsable<int*>::value, "");               // プリミティブ型ポインタ
    static_assert(not Udon::Traits::Parsable<int*[]>::value, "");             // プリミティブ型ポインタ配列
    static_assert(not Udon::Traits::Parsable<float*>::value, "");             // プリミティブ型ポインタ
    static_assert(not Udon::Traits::Parsable<float*[]>::value, "");           // プリミティブ型ポインタ配列
    static_assert(not Udon::Traits::Parsable<int&>::value, "");               // プリミティブ型参照
    static_assert(not Udon::Traits::Parsable<float&>::value, "");             // プリミティブ型参照
    static_assert(not Udon::Traits::Parsable<UnParsable>::value, "");         // パース不可能なユーザー定義型
    static_assert(not Udon::Traits::Parsable<InnerUnParsable>::value, "");    // 内部にパース不可能なメンバ変数を持つ構造体
}