# トラブルシューティング

## ■ エラーから索引

- リンクエラー「 -- は既に -- で定義されています。」

  - [ヘッダーに変数、関数を定義している](#-ヘッダーに変数関数を定義している)

- リンクエラー「未解決の外部シンボル -- が関数 -- で参照されました」

## ■ ヘッダーに変数、関数を定義している

```cpp
// Sample.hpp

int global;

void f() {}
```

### 原因

ヘッダーに変数の実体を定義して複数のソースファイルから使用した場合、多重定義でリンクエラーとなります。

単一のソースファイルから使用している場合、エラーにならない (エラーが隠れており後々発狂案件) になるので気を付けてください。

<details>
<summary> リンクエラーとなる原因の詳細 </summary>

例えば、次のように関数の定義が含まれるヘッダーを、2 つのソースファイルからインクルードした場合、

```cpp
// Sample.hpp
void f() {}

// a.cpp
#include "Sample.hpp"

// b.cpp
#include "Sample.hpp"

int main() {
    f();
}
```

プリプロセッサによって以下の様に展開されます。

```cpp
// a.cpp
void f() {}

// b.cpp
void f() {}

int main() {
    f();
}
```

次にコンパイラーがソースファイルを別々にコンパイルし、オブジェクトファイルを生成します。※逆アセンブル後のイメージ(不要部分を略しています)

```cpp
// a.o
f:

// b.o
f:

main:
        callq f
```

最後にリンカによって 1 つのバイナリにまとめられます。このとき関数の実体が 2 つあるため、どちらを呼べば判断できず多重定義となります。

</details>

### 解決策 1 (一般的)

ヘッダーに宣言を書き、ソースファイルに定義を書きます。

```cpp
// Sample.hpp

extern int global;
void f();
```

```cpp
// Sample.cpp

#include "Sample.hpp"

int global = 10;
void f() {}
```

### 解決策 2

インライン化するとヘッダーオンリーで書けます。

インライン関数は　インライン変数は C++17 以降で使用できます。C++17 以前の場合解決策 3 へ

```cpp
inline int global = 10;

inline void f() {}
```

### 解決策 3 (脳筋)

ヘッダーオンリーにしたい場合、関数の中に静的変数を用意し、参照を取り出すことでソースファイルが不要になります。

```cpp
// Sample.hpp

inline int& ValueRef()
{
    static int value;
    return value;
}

int& global = ValueRef();
```

### 解決策 4

constexpr 定数はヘッダーに書くことができます。

```cpp
// Sample.hpp
constexpr int global = 10;
```

## ■ 静的メンバ変数を使おうとしたが、「未解決の外部シンボル~が関数~で参照されました」というリンクエラーとなった。

```cpp
// Sample.hpp
class Sample
{
public:
    static int si;
};

// main.cpp
#include "Sample.hpp"

int main()
{
    Sample::si = 100;
}

// error LNK2019: 未解決の外部シンボル "public: static int Sample::si" (?si@Sample@@2HA) が関数 main で参照されました
```

クラス内に書かれた静的メンバ変数は変数の宣言に過ぎないため、実体をソースファイルに定義する必要があります。

> クラスの設計をしている段階で、静的メンバ変数を使わざる負えなくなったとき、設計ミスである可能性が高いです。

```cpp
// Sample.hpp
class Sample
{
    static int si;
};

// Sample.cpp
#include "Sample.hpp"
int Sample::si = 0;
```

ヘッダーオンリーにしたい場合は静的メンバ関数を定義し、内部に静的変数を持たせることで、クラス外に実体を定義する必要がなくなります。

```cpp
// Sample.hpp
class Sample
{
    Sample()
    {
        self() = this;
    }

    static Sample* self()
    {
        static Sample* p = nullptr;
        return p;
    }
};
```

## 謎のリンクエラーと

// error LNK2005: "void \_\_cdecl f(void)" (f) は既に a-d87a9b.o で定義されています。

通常の静的関数はリンクエラーを引き起こすため、ヘッダーに実装することが出来ません。これはインライン化によって対応しています。

```cpp
// *.hpp
inline void f() {}
```

<details>
<summary> リンクエラーとなる原因 </summary>

例えば、通常の静的関数が含まれるヘッダーを、2 つのソースファイルからインクルードした場合を考えます。

```cpp
// Sample.hpp
void f() {}

// a.cpp
#include "Sample.hpp"

// b.cpp
#include "Sample.hpp"
```

プリプロセッサによって以下の様に展開されます。

```cpp
// a.cpp
void f() {}

// b.cpp
void f() {}
```

次にコンパイラーがソースファイルを別々にコンパイルし、バイナリを生成します。

```cpp
// a.o
"f":

// b.o
"f":
```

最後にリンカによって 1 つのバイナリにまとめられます。このとき関数の実体が 2 つあるので、多重定義となります。

</details>

## ■ 型の相互変換編

### 問題

相互インクルードによって、インクルードが循環しコンパイルエラーとなる。

ユーザー定義型は型ごとにヘッダーに定義されます。相互変換を実現するには互いに型情報をする必要があるため、お互いをインクルードし合う状況になり、エラーとなります。

↓ 色空間の相互変換を行う例

```cpp
// RGB.hpp
#include "HSV.hpp"   // NO!

struct RGB {
    HSV toHSV() {
        return {};
    }
};
```

```cpp
// HSV.hpp
#include "RGB.hpp"   // NO!

struct HSV {
    RGB toRGB() {
        return {};
    }
};
```

### 解決

型の定義があるヘッダーに変換用関数の宣言、変換先の型の宣言を記述します。変換用関数は別のヘッダーに実装します。

```cpp
// RGBImpl.hpp
struct HSV;

struct RGB {
    HSV toHSV();
};
```

```cpp
// HSVImpl.hpp
struct RGB;

struct HSV {
    RGB toRGB();
};
```

```cpp
// RGB.hpp
#include "RGBImpl.hpp"
#include "HSVImpl.hpp"

HSV RGB::toHSV() {
    return {};
}
```

```cpp
// HSV.hpp
#include "HSVImpl.hpp"
#include "RGBImpl.hpp"

RGB HSV::toRGB() {
    return {};
}
```
