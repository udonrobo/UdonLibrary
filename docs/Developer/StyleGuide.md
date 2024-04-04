# スタイルガイド

本ライブラリへの機能追加、改良を行う際のガイドです。今後変更していただいても大丈夫です。

## 言語

- 使用言語

  `C++` を使用します。

- 言語バージョン

  共通部は `C++11` を使用します。`C++14` 以上の機能は現時点で使用していません。各マイコンに合わせた機能の場合は例外です。

  > 理由: Arduino Nano のコンパイラのバージョンが C++11 であり、低いバージョンに合わせているためです。Arduino Nano への対応を終了する場合、 `C++14` へのバージョンアップをしてもいいと思います。 (Teensy: C++14, RaspberryPiPico: C++14)

- 依存ライブラリ

  基本的に C 言語標準ライブラリもしくは C++ 標準ライブラリ STL(C+11) を使用します。

## 書式

[clang-format](https://clang.llvm.org/docs/ClangFormat.html) を使用してファイルを自動整形します。書式設定ファイルは `UdonLibrary/.clang-format` にあります。

## 命名規則

変数、関数などを作成する際の命名規則です。

### ディレクトリ(フォルダ)、ファイル名

`アッパーキャメルケース` を使用します。

拡張子はヘッダーファイルに `*.hpp`、ソースファイルに `*.cpp` を使用します。

> 例外的に `UdonLibrary/` 下のディレクトリ名には `ローワーキャメルケース` を使用しています。なぜなら、Arduino のライブラリディレクトリ名は `src/` である必要があり、これに合わせているためです。

```
UdonLibrary/
├─ src/
│  └─Driver/
│    └─Motor.hpp
```

### 変数 定数

- 通常変数 (ローカル、グローバル変数)

  `ローワーキャメルケース` を使用します。

  ```cpp
  // OK
  auto value = 100;
  auto nodeId = 0x1100;

  // NG
  auto Value = 100;
  auto node_id = 0x1100;
  ```

- メンバ変数

  通常の変数名と同じ。

- 実行時定数

  通常の変数名と同じ。

  ```cpp
  const auto value = F();
  ```

- コンパイル時定数

  `アッパーキャメルケース` を使用します。

  ```cpp
  constexpr auto Constant = 3 * 2;
  ```

### 関数

- グローバル関数

  `アッパーキャメルケース` を使用します。

  ```cpp
  // OK
  void BarFunc();

  // NG
  void barFunc();
  ```

- メンバ関数

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

### 型名

- クラス 構造体

  `アッパーキャメルケース` を使用します。

  ```cpp
  //OK
  class FooClass{};

  // NG
  class fooClass{};
  ```

- インターフェースクラス

  クラス名の先頭に `I` を加えます。

  ```cpp
  class IFooClass{};
  ```

- メンバ型

  `アッパーキャメルケース` を使用します。

  ```cpp
  class FooClass
  {
      using TypeAlias = int;
  };
  ```

- 列挙型

  型名、メンバ名ともに `アッパーキャメルケース` を使用します。

  ```cpp
  enum class FooEnum
  {
      FooBar,
      HogeHoge,
  };
  ```

- 共用体

  `アッパーキャメルケース` を使用します。メンバはクラスのメンバと同じ命名規則を使用します。

  ```cpp
  union FooUnion
  {
      int a;
      double b;
  };
  ```

### 名前空間

`アッパーキャメルケース` を使用します。

```cpp
// OK
namespace FooSpace {}

// NG
namespace fooSpace {}
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

- テンプレート引数

  `アッパーキャメルケース` を使用します。短い名前が好ましいです。

  ```cpp
  template <typename T>
  void Sample(const T& value)
  {
  }
  ```

  ```cpp
  template <typename T>
  class Sample
  {
  }
  ```

  ```cpp
  template <int N>
  void Sample(uint8_t (&array)[N])
  {
  }
  ```

- メタ関数

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
