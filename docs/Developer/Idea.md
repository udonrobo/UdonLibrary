## ■ 静的変数編

### 問題

ヘッダーに変数の実体を定義するとリンクエラーとなる。

### 解決

一般的にはヘッダーに宣言を書き、ソースファイルを用意して実体化します。しかしソースファイルは作りたくない...

```cpp
// *.hpp
extern int global;

// *.cpp
int global = 10;
```

関数の中に静的変数を用意し、参照を取り出すことでヘッダーに静的変数があるかのように書けます。

```cpp
// *.hpp

inline int& ValueRef()
{
    static int value;
    return value;
}

int& value = ValueRef();
```

ちなみに constexpr 定数はヘッダーに書くことができます。

```cpp
// *.hpp
constexpr int value = 10;
```

## ■ 静的関数編

通常の静的関数はリンクエラーを引き起こすため、ヘッダーに実装することが出来ません。これはインライン化によって対応しています。

```cpp
// *.hpp
inline void f() {}
```

<details>
<summary> リンクエラーとなる原因 </summary>

多重定義によってリンクエラーが起こります。

例えば通常の静的関数が含まれるヘッダーを、2 つのソースファイルからインクルードした場合を考えます。

```cpp
// sample.hpp
void f() {}

// a.cpp
#include "sample.hpp"

// b.cpp
#include "sample.hpp"
```

ビルド時、まずコンパイラーがソースファイルを別々にコンパイルし、バイナリを生成します。その後、リンカーによって 1 つのバイナリにまとめられます。

故にバイナリは 2 つ生成され、これらをリンクしたとき関数の実体が 2 つあるので、多重定義となります。

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
