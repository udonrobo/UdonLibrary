# StringView

文字列を保持せず、参照するクラスです。`std::string_view` と同義です。`std::string` と異なり、ヒープアロケーションが発生しないため効率の良い文字列操作を行えます。次のように実装されています。

```cpp
class StringView
{
    const char* data;
    size_t      size;
};
```

data ポインタは確保済みの領域を指すのみで、メモリの所有権 (メモリを解放する義務) を持ちません。

## 個別インクルード

```cpp
#include <Udon/Types/StringView.hpp>
```

## std::string との違い

`std::string` との違いが顕著なのは、文字列から細分化した文字列にする場合と思います。

```cpp
std::string src = "hello world !!";
std::string hello = src.substr(0, 5);  // hello
std::string world = src.substr(6, 5);  // world
```

std::string は動的にメモリを確保し文字列を保持します。そのため上記の例では 3 度、ヒープへのメモリアロケーションが発生します。

StringView の場合はどうでしょうか。

```cpp
Udon::StringView src = "hello world !!";
Udon::StringView hello = src.substr(0, 5);  // hello
Udon::StringView world = src.substr(6, 5);  // world
```

なんとヒープアロケーションは 0 回です。これは "hello world !!" という、データ領域に確保されている領域をポインタで指しているためです。そのため `std::string` と違い文字列の書き換えはできません。

## 標準出力への出力

`std::ostream` オブジェクトへの出力に対応しています。

```cpp
int main()
{
    Udon::StringView sv = "hello";
    std::cout << sv << std::endl;
}
```

## サイズ

```cpp
Udon::StringView sv = "hello";

size_t size = sv.size();  // 5

bool empty = sv.empty();  // false
```

## 文字アクセス

```cpp
Udon::StringView sv = "hello";

char front = sv.front();  // h

char back = sv.back();  // 0

char l = sv.at(2);  // l

char h = sv[0];  // h

const char* data = sv.data();  // 文字列先頭ポインタ
```

## 部分文字列作成

### ■ 指定された終端文字までの部分文字列を作成

```cpp
Udon::StringView sv = "hello world\n hogehoge";

Udon::StringView result = sv.substrUntil('\n');

// result: { "hello world" }
```

### ■ 先頭の N 文字を削除した部分文字列を作成する

```cpp
Udon::StringView sv = "message: error";

Udon::StringView result = sv.removePrefix(9);

// result: { "error" }
```

### ■ 末尾の N 文字を削除した部分文字列を作成する

```cpp
Udon::StringView sv = "message: error";

Udon::StringView result = sv.removeSuffix(7);

// result: { "message" }
```

### ■ 指定された区切り文字で区切り部分文字列のリストを作成する

```cpp
Udon::StringView sv = "Hello I am Japanese";

std::vector<Udon::StringView> result = sv.split(' ');

// result: { "Hello", "I", "am", "Japanese" }
```

## 文字列比較

### ■ 単純な文字列比較

```cpp
Udon::StringView a = "hello";
Udon::StringView b = "hello";

bool eq = a == b;  // true
bool ne = a != b;  // false
```

### ■ 指定の文字列で始まっているか判定

```cpp
Udon::StringView src = "hello world";
bool result = src.startsWith("hello");   // result: true
```

### ■ 指定の文字列で終わっているか判定

```cpp
Udon::StringView src = "hello world";
bool result = src.endsWith("world");   // result: true
```

## イテレータ

イテレータを定義しているため for 文などでイテレーションできます。

```cpp
Udon::StringView sv = "hello";

for (char c : sv)
{
}
```

```cpp
Udon::StringView sv = "hello world";

auto it = std::find(sv.begin(), sv.end(), 'w');

if (it != sv.end())
{
    int index = std::distance(sv.begin(), it);
    // index: 6
}
```

## 数値への変換

```cpp
Udon::StringView string = "1234";

if (Udon::Optional<int> numopt = string.toNumber<int>())
{
    int number = *numopt;
    // number: 1234
}
```

```cpp
Udon::StringView string = "Double: 1234.12";

if (Udon::Optional<double> numopt = string.removePrefix(8).toNumber<double>())
{
    double number = *numopt;
    // number: 1234.12
}
```

## 入れ替え

```cpp
Udon::StringView sv1 = "hello";
Udon::StringView sv2 = "world";

std::swap(sv1, sv2);

// sv1: { "world" }
// sv2: { "hello" }
```

```cpp
Udon::StringView sv1 = "hello";
Udon::StringView sv2 = "world";

sv1.swap(sv2);

// sv1: { "world" }
// sv2: { "hello" }
```

## 文字列リテラル

```cpp
using namespace Udon::Literals;

auto sv = "hello world"_sv;
```

## std::string への変換

```cpp
Udon::StringView sv = "hello";

std::string str = sv.toString();
```
