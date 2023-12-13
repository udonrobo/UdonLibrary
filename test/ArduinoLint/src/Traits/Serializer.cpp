#include <Arduino.h>
#include <Udon/Traits/EnumerableMacro.hpp>
#include <Udon/Traits/SerializerTraits.hpp>

/// @brief 列挙型
enum class Enum
{
    A,
    B,
    C,
};

/// @brief パース不可能なユーザー定義型
struct UnEnumerable
{
    int a;
    int b;
    int c;
};

/// @brief  パース可能なユーザー定義型
struct Enumerable
{
    int a;
    int b;
    int c;
    UDON_ENUMERABLE(a, b, c);
};

/// @brief  内部にバース可能なメンバ変数を持つユーザー定義型
struct InnerEnumerable
{
    Enumerable a;
    Enumerable b;
    Enumerable c;
    UDON_ENUMERABLE(a, b, c);
};

/// @brief 内部にパース不可能なメンバ変数を持つユーザー定義型
struct InnerUnEnumerable
{
    UnEnumerable a;
    UnEnumerable b;
    UnEnumerable c;
};

inline void test()
{
    // パース可能
    static_assert(Udon::Traits::IsSerializable<int>::value, "");                    // プリミティブ型
    static_assert(Udon::Traits::IsSerializable<int[2][2]>::value, "");              // プリミティブ型多次元配列
    static_assert(Udon::Traits::IsSerializable<float>::value, "");                  // プリミティブ型
    static_assert(Udon::Traits::IsSerializable<float[2][2]>::value, "");            // プリミティブ型多次元配列
    static_assert(Udon::Traits::IsSerializable<Enum>::value, "");                   // 列挙型
    static_assert(Udon::Traits::IsSerializable<Enum[2][2]>::value, "");             // 列挙型多次元配列
    static_assert(Udon::Traits::IsSerializable<Enumerable>::value, "");               // ユーザー定義型
    static_assert(Udon::Traits::IsSerializable<Enumerable[2][2]>::value, "");         // ユーザー定義型多次元配列
    static_assert(Udon::Traits::IsSerializable<InnerEnumerable>::value, "");          // 内部にパース可能なメンバ変数を持つ構造体
    static_assert(Udon::Traits::IsSerializable<InnerEnumerable[2][2]>::value, "");    // 内部にパース可能なメンバ変数を持つ構造体多次元配列

    // 空配列はパース不可
    static_assert(not Udon::Traits::IsSerializable<int[]>::value, "");         // プリミティブ型配列
    static_assert(not Udon::Traits::IsSerializable<float[]>::value, "");       // プリミティブ型配列
    static_assert(not Udon::Traits::IsSerializable<Enumerable[]>::value, "");    // ユーザー定義型配列

    // パース不可
    static_assert(not Udon::Traits::IsSerializable<int*>::value, "");               // プリミティブ型ポインタ
    static_assert(not Udon::Traits::IsSerializable<int*[]>::value, "");             // プリミティブ型ポインタ配列
    static_assert(not Udon::Traits::IsSerializable<float*>::value, "");             // プリミティブ型ポインタ
    static_assert(not Udon::Traits::IsSerializable<float*[]>::value, "");           // プリミティブ型ポインタ配列
    static_assert(not Udon::Traits::IsSerializable<int&>::value, "");               // プリミティブ型参照
    static_assert(not Udon::Traits::IsSerializable<float&>::value, "");             // プリミティブ型参照
    static_assert(not Udon::Traits::IsSerializable<UnEnumerable>::value, "");         // パース不可能なユーザー定義型
    static_assert(not Udon::Traits::IsSerializable<InnerUnEnumerable>::value, "");    // 内部にパース不可能なメンバ変数を持つ構造体
}