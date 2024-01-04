# スタイルガイド

本ライブラリへの機能追加、改良を行う際のガイドです。ライブラリ機能のスタイルが統一されていると、今後変更していただいても大丈夫です。

## 言語

- 使用言語

  `C++` を使用します。

- 言語バージョン

  `C++11` を使用します。`C++14` 以上の機能は現時点で使用していません。

  > 理由: Arduino Nano のコンパイラのバージョンが C++11 であるためです。Arduino Nano への対応を終了する場合、 `C++14` へのバージョンアップをしてもいいと思います。 (Teensy: C++14, RaspberryPiPico: C++14)

## 書式

[clang-format](https://clang.llvm.org/docs/ClangFormat.html) を使用してファイルを自動整形します。書式設定ファイルは `UdonLibrary/.clang-format` にあります。

## 命名規則

変数、関数などを作成する際の命名規則です。

### ディレクトリ(フォルダ)、ファイル名

`アッパーキャメルケース` を使用します。

拡張子はヘッダーファイルに `*.hpp`、ソースファイルに `*.cpp` を使用します。

> 例外的に `UdonLibrary/` 下のディレクトリ名には `ローワーキャメルケース` を使用しています。なぜなら、Arduino のライブラリディレクトリ名は `src/` である必要があり、これに合わせているためです。

```
├─ FooDir/
│  └─BarDir/
│    └─BaaSample.hpp
```

### 変数名

`ローワーキャメルケース` を使用します。

```cpp
// OK
auto value = 100;
auto nodeId = 0x1100;

// NG
auto Value = 100;
auto node_id = 0x1100;
```

### グローバル関数名

`アッパーキャメルケース` を使用します。

```cpp
// OK
void BarFunc();

// NG
void barFunc();
```

### 名前空間名

`アッパーキャメルケース` を使用します。

```cpp
// OK
namespace FooSpace
{
}

// NG
namespace fooSpace
{
}
```

### クラス名

`アッパーキャメルケース` を使用します。

```cpp
//OK
class FooClass{};

// NG
class fooClass{};
```

### インターフェースクラス名

クラス名の先頭に `I` を加えます。

```cpp
class IFooClass{};
```

### メンバ変数名

通常の変数名と同じ。

### メンバ関数名

`ローワーキャメルケース` を使用します。

```cpp
class FooClass
{
    // OK
    void barFunc();

    // NG
    void bar_func();
};
```

### メンバ型名

`アッパーキャメルケース` を使用します。

標準ライブラリ側から使用される型名は例外的に `スネークケース` を使用しています (`value_type` 等)。

```cpp
class FooClass
{
    // OK
    using TypeAlias = int;

    // NG
    using type_alias = int;
};
```

### 列挙型

型名、メンバ名ともに `アッパーキャメルケース` を使用します。

```cpp
// OK
enum class FooEnum
{
    FooBar,
    HogeHoge,
};

// NG
enum class fooEnum
{
    FOO_BAR,
    hogeHoge,
};
```

### コンパイル時に定数になる定数

`大文字のスネークケース` を使用します。

```cpp
// OK
constexpr auto CONSTANT = 3 * 2;

// NG
constexpr auto constant = 3 * 2;
```

### 実行時に定数になる定数

通常の変数名と同じ。

```cpp
int main()
{
    // OK
    const value = f();

    // NG
    const VALUE = f();
}
```

### マクロ

`大文字のスネークケース` を使用します。

```cpp
// OK
#define FOO_MACRO 100

// NG
#define FooMacro 100
```

### テンプレート

- テンプレート引数名

  `アッパーキャメルケース` を使用します。短い名前が好ましいです。

  ```cpp
  template <typename T>
  void f(const T& value)
  {
  }
  ```

  ```cpp
  template <int N>
  void f(uint8_t (&array)[N])
  {
  }
  ```

- メタ関数名

  `アッパーキャメルケース` を使用します。型の特性を取得するものなので、`Is~` や `~able` を使用すると良いです。

  ```cpp
  template <typename T>
  struct IsBool : std::is_same<T, bool>
  {
  };
  ```

  ```cpp
  template <typename T>
  struct IsSerializable : //...
  {
  };
  ```
