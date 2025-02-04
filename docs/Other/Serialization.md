# シリアライザ

オブジェクトのバイト列化、バイト列からオブジェクトへの復元を行います。主に通信クラス内部で使用されています。

> 使用例
>
> ```mermaid
> flowchart LR
>     subgraph マイコン
>     まいこんおぶ[オブジェクト] --シリアライズ--> a[バイト列]
>     end
>     subgraph パソコン
>     a[バイト列] --> バイト列 --デシリアライズ--> ぱそこんおぶ[オブジェクト]
>     end
> ```

## 個別インクルード

```cpp
#include <Udon/Serializer/Serializer.hpp>
```

## シリアライズ可能な型

整数型 ( `int`, `uint8_t`, `uint16_t` ... )

浮動小数点型 ( `double`, `float` ... )

列挙型 ( `enum`, `enum class` )

シリアライズ可能な型の配列 (メンバ変数のみ)

列挙可能なユーザー定義型

## シリアライズ

シリアライズしたいオブジェクトのデータ構造を構造体、クラスを用いて定義します。`UDON_ENUMERABLE` マクロにメンバ変数を登録することで、メンバ変数を走査できるようになり、シリアライズできるようになります。

`Udon::Serialize(object)` の引数にオブジェクトを渡すことでバイト列 `std::vector<uint8_t>` が返されます。

```cpp
struct Vec2
{
    double x;
    double y;
    UDON_ENUMERABLE(x, y);
};

void setup() {}

void loop()
{
    Vec2 v{ 2.5, 3.4 };
    const std::vector<uint8_t> packed = Udon::Serialize(v);
}
```

## デシリアライズ

バイト列をオブジェクトに復元するには `Udon::Deserialize<T>(...)` を使用します。 `Udon::Deserialize` はデシリアライズされたオブジェクトを `Udon::Optional` でラップして返します。

`Optional` 型とは値とエラー値を持つ型で、`Optional::operator bool()` を呼び出すことで次のように if 文でエラー判定可能です。

> バイト列末端バイトに挿入されているチェックサムと、バイト列から求めたチェックサムが異なる場合、デシリアライズが失敗します。
> チェックサムの整合が取れない場合、データが破損しています。

```cpp
void loop()
{
    // packed にはシリアライズされたバイト列が入っている
    if (const Udon::Optional<Vec2> unpacked = Udon::Deserialize<Vec2>(packed))
    {
        Serial.print(unpacked->x);  Serial.print('\t');
        Serial.print(unpacked->y);  Serial.print('\n');
    }
    else
    {
        Serial.println("Deserialize failed!");  // データ破損
    }
}
```

## 既に定義されている型のシリアライズ、デシリアライズ

> 作成中

<!-- **クローバル空間** に `Capacity(const T&)` `void Accessor<Acc>(Acc&, T&)` 関数を次のように定義することで、ライブラリ等で既に定義されている型をシリアライズできます。

```cpp
// ↓ 外部ライブラリ内に定義されている Vec2
struct Vec2
{
    double x;
    double y;
}

// シリアライズ後のビット数をコンパイル時に取得する関数
constexpr size_t Capacity(const Vec2& rhs)
{
    return Udon::SerializedBitSize(rhs.x, rhs.y);
}

// シリアライザ、デシリアライザが使用するメンバ変数解析用関数
template <typename Acc>
void Accessor(Acc& acc, Vec2& rhs)
{
    acc(rhs.x, rhs.y);
};
``` -->

## 詳細

バイト列末尾に CRC8 値を付与します。

bool 型は 1bit としてシリアライズします。

浮動小数点型はアーキテクチャによってサイズが異なるので、常に 32bit 浮動小数点型にキャストして扱います。

列挙型は基底型を基にシリアライズします。

エンディアン変換を自動的に行います。リトルエンディアン環境を順列として扱い、ビッグエンディアン環境の場合はエンディアン変換を行います。

## API

### `Udon::Serialize(...)`

オブジェクトをシリアライズします

Serialize 関数は次のオーバーロードが定義されています。`buffer` を引数にとる関数はヒープ領域を使用しません。

```cpp
std::vector<uint8_t> Serialize(const T& object);

bool Serialize(const T& object, ArrayView<uint8_t> buffer);
```

### `Udon::Deserialize<T>(...)`

バイト列をオブジェクトにデシリアライズします

```cpp
template <typename T>
Udon::Optional<T> Deserialize(ArrayView<const uint8_t> buffer);
```

### `Udon::IsDeserializable(...)`

デシリアライズできるかを確認します。(CRC 値の一致を確認)

```cpp
bool IsDeserializable(ArrayView<const uint8_t> buffer);
```

### `Udon::SerializedSize<T>()`

T 型オブジェクトシリアライズ後のバイト列のバイトサイズを取得します。

コンパイル時にサイズを取得可能なため、バッファの大きさを静的に指定するときに使用します。

```cpp
uint8_t buffer[Udon::SerializedSize<Vec2>()];
```

## サンプル

### 整数型シリアライズ

```cpp
#include <iostream>
#include <Udon/Serializer/Serializer.hpp>

int main()
{
    const auto packed = Udon::Serialize(1000);

    if (const auto unpacked = Udon::Deserialize<int>(packed))
    {
        std::cout << *unpacked << std::endl;
    }
    else
    {
        std::cout << "deserialize failed" << std::endl;
    }
}
```

```
100
```

### ヒープ領域を使用しないシリアライズ

`Udon::Serialize(T) -> std::vector<uint8_t>` は `std::vector` を用いており、バイト列をヒープ領域に割り当てて返します。パフォーマンスを気にする場合、ヒープ領域の使用はなるべく控えるべきです。`Udon::Serialize(T, Udon::ArrayView<uint8_t>)`を用いることで、スタック領域(静的配列)をバッファーとしてシリアライズできます。

この時、バッファーのサイズと、シリアライズ後のサイズ(`Udon::SerializedSize<T>()`)が異なる場合 `Udon::Serialize` は失敗し false を返します。

```cpp
#include <iostream>
#include <Udon/Serializer/Serializer.hpp>

int main()
{
    uint8_t buffer[Udon::SerializedSize<int>()];

    if (not Udon::Serialize(1000, buffer))
    {
        std::cout << "serialize failed" << std::endl;
    }

    if (const auto unpacked = Udon::Deserialize<int>(buffer))
    {
        std::cout << *unpacked << std::endl;
    }
    else
    {
        std::cout << "deserialize failed" << std::endl;
    }
}
```

```
1000
```

### 列挙型シリアライズ

```cpp
#include <iostream>
#include <Udon/Serializer/Serializer.hpp>

enum class Language : uint8_t
{
    C,
    Cpp,
    CSharp,
    Python,
};

int main()
{
    const Language lang = Language::Cpp;

    const auto packed = Udon::Serialize(lang);

    if (const auto unpacked = Udon::Deserialize<Language>(packed))
    {
        switch (*unpacked)
        {
        case Language::C     : std::cout << "C"      << std::endl; break;
        case Language::Cpp   : std::cout << "C++"    << std::endl; break;
        case Language::CSharp: std::cout << "C#"     << std::endl; break;
        case Language::Python: std::cout << "Python" << std::endl; break;
        }
    }
    else
    {
        std::cout << "deserialize failed" << std::endl;
    }
}
```

```
C++
```

### ユーザー定義型シリアライズ

```cpp
#include <iostream>
#include <Udon/Serializer/Serializer.hpp>

struct Vec2
{
    double x;
    double y;
    UDON_ENUMERABLE(x, y);
};

int main()
{
    const Vec2 vector{ 1.0, 2.0 };

    const auto packed = Udon::Serialize(vector);

    if (const auto unpacked = Udon::Deserialize<Vec2>(packed))
    {
        std::cout
            << unpacked->x << ", "
            << unpacked->y << std::endl;
    }
    else
    {
        std::cout << "deserialize failed" << std::endl;
    }
}
```

```
1, 2
```

### メンバに配列を持つユーザー定義型

```cpp
#include <iostream>
#include <Udon/Serializer/Serializer.hpp>

struct Array
{
    int array[5];
    UDON_ENUMERABLE(array);
};

int main()
{
    Array array{ { 1, 2, 3, 4, 5 } };

    const auto packed = Udon::Serialize(array);

    if (const auto unpacked = Udon::Deserialize<Array>(packed))
    {
        for (const auto& e : unpacked->array)
        {
            std::cout << e << std::endl;
        }
    }
    else
    {
        std::cout << "deserialize failed" << std::endl;
    }
}
```

```
1
2
3
4
5
```

### ユーザー定義型のネスト

```cpp
#include <iostream>
#include <Udon/Serializer/Serializer.hpp>

struct Vec2
{
    struct Double
    {
        double value;
        UDON_ENUMERABLE(value);
    };

    Double x;
    Double y;

    UDON_ENUMERABLE(x, y);
};

int main()
{
    const Vec2 vector{ { 1.0 }, { 2.0 } };

    const auto packed = Udon::Serialize(vector);

    if (const auto unpacked = Udon::Deserialize<Vec2>(packed))
    {
        std::cout
            << unpacked->x.value << ", "
            << unpacked->y.value << std::endl;
    }
    else
    {
        std::cout << "deserialize failed" << std::endl;
    }
}
```

```
1, 2
```

### デシリアライズ失敗

```cpp
#include <iostream>
#include <Udon/Serializer/Serializer.hpp>

int main()
{
    auto packed = Udon::Serialize(1000);

    packed.at(1) = 0x00;  // データ破損

    if (const auto unpacked = Udon::Deserialize<int>(packed))
    {
        std::cout << *unpacked << std::endl;
    }
    else
    {
        std::cout << "deserialize failed" << std::endl;
    }
}
```

```
deserialize failed
```
