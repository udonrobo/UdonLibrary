# シリアライザ

```cpp
#include <Udon/Com/Serialization.hpp>
```

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

## シリアライズ可能な型

整数型 ( `int`, `uint8_t`, `uint16_t` ... )

浮動小数点型 ( `double`, `float` ... )

列挙型 ( `enum`, `enum class` )

シリアライズ可能な型の配列 (メンバ変数のみ)

パース可能なユーザー定義型

## シリアライズ

シリアライズしたいオブジェクトのデータ構造を構造体、クラスを用いて定義します。`UDON_PARSABLE` マクロにメンバ変数を登録することで、メンバ変数を走査できるようになり、シリアライズできるようになります。

`Udon::Pack(object)` の引数にオブジェクトを渡すことでバイト列 `std::vector<uint8_t>` が返されます。

```cpp
struct Vec2
{
    double x;
    double y;
    UDON_PARSABLE(x, y);
};

void setup() {}

void loop()
{
    Vec2 v{ 2.5, 3.4 };
    const std::vector<uint8_t> packed = Udon::Pack(v);
}
```

## デシリアライズ

バイト列をオブジェクトに復元するには `Udon::Unpack<T>(...)` を使用します。 `Udon::Unpack` はデシリアライズされたオブジェクトを `Udon::Optional` でラップして返します。

`Optional` 型とは値とエラー値を持つ型で、`Optional::operator bool()` を呼び出すことで次のように if 文でエラー判定可能です。

> バイト列末端バイトに挿入されているチェックサムと、バイト列から求めたチェックサムが異なる場合、デシリアライズが失敗します。
> チェックサムの整合が取れない場合、データが破損しています。

```cpp
void loop()
{
    // packed にはシリアライズされたバイト列が入っている
    if (const Udon::Optional<Vec2> unpacked = Udon::Unpack<Vec2>(packed))
    {
        Serial.print(unpacked->x);  Serial.print('\t');
        Serial.print(unpacked->y);  Serial.print('\n');
    }
    else
    {
        Serial.println("Unpack failed!");  // データ破損
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
    return Udon::PackedBitSize(rhs.x, rhs.y);
}

// シリアライザ、デシリアライザが使用するメンバ変数解析用関数
template <typename Acc>
void Accessor(Acc& acc, Vec2& rhs)
{
    acc(rhs.x, rhs.y);
};
``` -->

## 詳細

バイト列末尾にチェックサム (CRC8) を付与します。

bool 型は 1bit としてシリアライズします。

浮動小数点型はアーキテクチャによってサイズが異なるので、32bit 浮動小数点型にキャストして扱います。

列挙型は基底型を基にシリアライズします。

エンディアン変換を自動的に行います。

## API

### `Udon::Pack(...)`

オブジェクトをシリアライズします

Pack 関数は次のオーバーロードが定義されています。

```cpp
std::vector<uint8_t> Pack(const T& object)

bool Pack(const T& object, uint8_t* buffer, size_t size)

bool Pack(const T& object, uint8_t (&array)[N])
```

### `Udon::Unpack<T>(...)`

バイト列をオブジェクトにデシリアライズします

Unpack 関数は次のオーバーロードが定義されています。

```cpp
template <typename T>
Udon::Optional<T> Unpack(const std::vector<uint8_t>& buffer)

template <typename T>
Udon::Optional<T> Unpack(const uint8_t* buffer, size_t size)

template <typename T>
Udon::Optional<T> Unpack(const uint8_t (&array)[N])
```

### `Udon::CanUnpack(...)`

デシリアライズできるかを確認します。(チェックサム確認)

```cpp
bool CanUnpack(const std::vector<uint8_t>& buffer)

bool CanUnpack(const uint8_t* buffer, size_t size)

bool CanUnpack(const uint8_t (&array)[N])
```

### `Udon::PackedSize<T>()`

T 型オブジェクトシリアライズ後のバイト列のバイトサイズを取得します。

コンパイル時にサイズを取得可能なため、バッファの大きさを静的に指定するときなどにも使えます。

```cpp
uint8_t buffer[Udon::PackedSize<Vec2>()];
```

### `Udon::PackedBitSize(...)`

オブジェクトをシリアライズした際のバイト列のビットサイズを取得します。(bool 型 を 1bit としてカウントするため)

`Capacity` 関数を外部に定義するときに使用します。引数は可変長引数です。

## サンプル

### 整数型シリアライズ

```cpp
#include <iostream>
#include <Udon/Com/Serialization.hpp>

int main()
{
	const auto packed = Udon::Pack(1000);

	if (const auto unpacked = Udon::Unpack<int>(packed))
	{
		std::cout << *unpacked << std::endl;
	}
	else
	{
		std::cout << "unpack failed" << std::endl;
	}
}
```

```
100
```

### 列挙型シリアライズ

```cpp
#include <iostream>
#include <Udon/Com/Serialization.hpp>

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

	const auto packed = Udon::Pack(lang);

	if (const auto unpacked = Udon::Unpack<Language>(packed))
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
		std::cout << "unpack failed" << std::endl;
	}
}
```

```
C++
```

### ユーザー定義型シリアライズ

```cpp
#include <iostream>
#include <Udon/Com/Serialization.hpp>

struct Vec2
{
	double x;
	double y;
	UDON_PARSABLE(x, y);
};

int main()
{
	const Vec2 vector{ 1.0, 2.0 };

	const auto packed = Udon::Pack(vector);

	if (const auto unpacked = Udon::Unpack<Vec2>(packed))
	{
		std::cout
            << unpacked->x << ", "
            << unpacked->y << std::endl;
	}
	else
	{
		std::cout << "unpack failed" << std::endl;
	}
}
```

```
1, 2
```

### メンバに配列を持つユーザー定義型

```cpp
#include <iostream>
#include <Udon/Com/Serialization.hpp>

struct Array
{
	int array[5];
	UDON_PARSABLE(array);
};

int main()
{
	Array array{ { 1, 2, 3, 4, 5 } };

	const auto packed = Udon::Pack(array);

	if (const auto unpacked = Udon::Unpack<Array>(packed))
	{
		for (const auto& e : unpacked->array)
		{
			std::cout << e << std::endl;
		}
	}
	else
	{
		std::cout << "unpack failed" << std::endl;
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
#include <Udon/Com/Serialization.hpp>

struct Vec2
{
	struct Double
	{
		double value;
		UDON_PARSABLE(value);
	};

	Double x;
	Double y;

	UDON_PARSABLE(x, y);
};

int main()
{
	const Vec2 vector{ { 1.0 }, { 2.0 } };

	const auto packed = Udon::Pack(vector);

	if (const auto unpacked = Udon::Unpack<Vec2>(packed))
	{
		std::cout
			<< unpacked->x.value << ", "
			<< unpacked->y.value << std::endl;
	}
	else
	{
		std::cout << "unpack failed" << std::endl;
	}
}
```

```
1, 2
```

### デシリアライズ失敗

```cpp
#include <iostream>
#include <Udon/Com/Serialization.hpp>

int main()
{
	auto packed = Udon::Pack(1000);

	packed.at(1) = 0x00;  // データ破損

	if (const auto unpacked = Udon::Unpack<int>(packed))
	{
		std::cout << *unpacked << std::endl;
	}
	else
	{
		std::cout << "unpack failed" << std::endl;
	}
}
```

```
unpack failed
```
