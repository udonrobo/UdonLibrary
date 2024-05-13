#include <Arduino.h>
#include <Udon/Traits/EnumerableMacro.hpp>
#include <Udon/Serializer/SerializerTraits.hpp>

/// @brief 列挙型
enum class Enum
{
    A,
    B,
    C,
};

/// @brief 列挙型を持つ構造体
struct HasEnumClassStruct
{
    enum class A
    {
        A,
    } a;
    UDON_ENUMERABLE(a);
};

struct HasArrayStruct
{
    int a[3];
    UDON_ENUMERABLE(a);
};

/// @brief 列挙不可能なユーザー定義型
struct UnEnumerable
{
    int a;
    int b;
    int c;
};

/// @brief  列挙可能なユーザー定義型
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

/// @brief 内部に列挙不可能なメンバ変数を持つユーザー定義型
struct InnerUnEnumerable
{
    UnEnumerable a;
    UnEnumerable b;
    UnEnumerable c;
};

inline void test()
{
    using namespace Udon::Traits;

    // 列挙可能
    static_assert(IsSerializable<int>::value, "");                   // プリミティブ型
    static_assert(IsSerializable<float>::value, "");                 // プリミティブ型
    static_assert(IsSerializable<Enum>::value, "");                  // 列挙型
    static_assert(IsSerializable<HasEnumClassStruct>::value, "");    // 列挙型を持つ構造体
    static_assert(IsSerializable<HasArrayStruct>::value, "");        // 配列型を持つ構造体
    static_assert(IsSerializable<Enumerable>::value, "");            // ユーザー定義型
    static_assert(IsSerializable<InnerEnumerable>::value, "");       // 内部に列挙可能なメンバ変数を持つ構造体

    // 空配列は列挙不可
    static_assert(not IsSerializable<int[]>::value, "");           // プリミティブ型配列
    static_assert(not IsSerializable<float[]>::value, "");         // プリミティブ型配列
    static_assert(not IsSerializable<Enumerable[]>::value, "");    // ユーザー定義型配列

    // 列挙不可
    static_assert(not IsSerializable<UnEnumerable>::value, "");         // 列挙不可能なユーザー定義型
    static_assert(not IsSerializable<InnerUnEnumerable>::value, "");    // 内部に列挙不可能なメンバ変数を持つ構造体
}