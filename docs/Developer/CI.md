# 自動テスト

GitHub Actions を使用して、プッシュ時、プルリクエスト作成時にライブラリのコードを自動的に検証します。

この検証を行うことで、ライブラリ追加時にコンパイルエラーになったり、バグが生じることを大幅に減らすことができます。

> テストコードのビルドに失敗、もしくは終了コードが 0 以外の場合、検証が失敗します。
>
> 検証の状態は [Action タブ](https://github.com/udonrobo/UdonLibrary/actions) から見ることができます。ワークフローは `UdonLibrary/.github/workflows/` に定義されています。
>
> またのメインページの README に貼られているバッジから見ることもできます(反映に少し時間がかかります)。
>
> [![Arduino Lint](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml) > [![Unit Tests](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml)
>
> 検証が成功している場合 `passing` と表示され、失敗していると `failing` と表示されます。

## Arduino Lint

[arduino cli](https://github.com/arduino/arduino-cli) を使用してコンパイルできるか検証します。

検証されるボードは `Arduino Nano` `Raspberry Pi Pico` `Teensy4.0` です。

### サブディレクトリ

`src` ディレクトリ内のファイルは再帰的にコンパイルされるため、`src` ディレクトリ下のファイルも自動的に検証されます。

### テストファイル

`UdonLibrary/test/ArduinoLint/src` に `.cpp` ファイルを作成します。

このテストでは構文のチェックを行うだけで、アルゴリズムのチェックは行いません。

```cpp
// UdonLibrary/src/Udon/Sample/Sample.hpp (例)
struct Sample
{
    double a;
    double f() const;
};
```

```cpp
// UdonLibrary/test/ArduinoLint/src/Sample/Sample.cpp (例)
#include <UdonFwd.hpp>
#include <Udon/Sample/Sample.hpp>

inline void test()
{
    Sample sample{ 10.0 };
    sample.f();
}
```

> `test` 関数は同じ名前の関数が他のソースファイルにも存在しています。そのためリンクエラーを起こさないようにインライン関数にします。

### コンパイル時に計算可能なアルゴリズムのテスト

コンパイル時に値が決定する関数、定数は `static_assert(定数式, 文字列リテラル)` を使用してコンパイル時にチェックすることができます。

> `static_assert` は定数式が `false` であるときにコンパイルエラーになります。

```cpp
// UdonLibrary/src/Udon/Algorithm/Factorial.hpp (例)
constexpr int Factorial(int n);
```

```cpp
// UdonLibrary/test/ArduinoLint/src/Algorithm/Factorial.cpp (例)
#include <UdonFwd.hpp>
#include <Udon/Algorithm/Factorial.hpp>

inline void test()
{
    static_assert(Factorial(1) ==   1, "");
    static_assert(Factorial(2) ==   2, "");
    static_assert(Factorial(3) ==   6, "");
    static_assert(Factorial(4) ==  24, "");
    static_assert(Factorial(5) == 120, "");
}
```

### 検証ボード追加

本テストの GitHub Actions のワークフローが `UdonLibrary/.github/workflows/ArduinoLint.yml` に定義されています。

このファイルにある `matrix: bord:` に `fqbn` を調べ追加することでボードの追加ができます。

```yml
- fqbn: rp2040:rp2040:rpipico
  platform: rp2040:rp2040
  url: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

### ローカル環境で実行

`UdonLibrary/test/ArduinoLint/ArduinoLint.ino` を ArduinoIDE で開き、検証ボタンを押すことで、プッシュせずにテストを実行できます。

## Google Unit Test

[GoogleTest](https://github.com/google/googletest) を使用してアルゴリズムの検証を行います。

### ディレクトリ追加

GoogleTest は CMake を使用してビルドを行います。そのためビルドしたいファイル、ディレクトリを CMakeLists.txt に登録する必要があります。

ディレクトリを追加する場合、追加した親ディレクトリ内の `CMakeLists.txt` に `add_subdirectory` を使用してディレクトリを登録します。

```cmake
# ./test/UnitTest/CMakeLists.txt (親ディレクトリのCMakeLists.txt)
# ...
# ...
# ファイル末尾
# サブディレクトリ登録
add_subdirectory(./Sample)
```

### ソースファイル追加

`UdonLibrary/test/UnitTest` にディレクトリを追加し、 `.cpp` ファイルを追加します。

ソースファイルを追加する場合、追加したディレクトリ内の `CMakeLists.txt` に `add_executable` を使用してソースファイルを登録します。

加えて GoogleTest とのリンク設定、本ライブラリのインクルードパス設定を行います。

```cmake
# UdonLibrary/test/UnitTest/Sample/CMakeLists.txt (子ディレクトリのCMakeLists.txt)
cmake_minimum_required(VERSION 3.14)

# ソースファイル登録
add_executable(SampleTest
    Sample.cpp
    # ...
    # 複数ソースファイルがある場合はここに追加していく
    # ...
)

# インクルードパス設定
target_include_directories(SampleTest PUBLIC ${UDON_LIBRARY_DIR})

# GoogleTestとリンク
target_link_libraries(SampleTest gtest_main)

# discover tests
gtest_discover_tests(SampleTest)
```

> `${UDON_LIBRARY_DIR}` は実行マシンから見た `UdonLibrary/src` の絶対パス名で `UdonLibrary/test/UnitTest/CMakeLists.txt` 内で定義されます。

### テストを書く

テストコードの書き方は [GoogleTest ドキュメント](https://google.github.io/googletest/reference/testing.html) を参照してください。

```cpp
// UdonLibrary/src/Udon/Algorithm/Factorial.hpp
int Factorial(int n);
```

```cpp
// UdonLibrary/test/UnitTest/Sample/Sample.cpp
#include <gtest/gtest.h>
#include <Udon/Algorithm/Factorial.hpp>

TEST(FactorialTest, HandlesPositiveInput)
{
    EXPECT_EQ(Factorial(1),   1);
    EXPECT_EQ(Factorial(2),   2);
    EXPECT_EQ(Factorial(3),   6);
    EXPECT_EQ(Factorial(4),  24);
    EXPECT_EQ(Factorial(5), 120);
}
```

> `EXPECT_EQ(val1, val2)` は `val1 == val2` であるかを検証します。

### ローカル環境で実行

`UdonLibrary/test/UnitTest` で以下のコマンドを実行することで、プッシュせずにローカルで実行できます。

```sh
cmake -S . -B Build
cmake --build Build
cd Build
ctest
cd ..
```

> CMake のインストールをあらかじめ済ませておく必要があります。
>
> ```sh
> # Windows
> winget install -e --id Kitware.CMake
> ```
>
> ```sh
> # MacOS Linux
> brew install cmake
> ```
>
> ```sh
> # Linux
> sudo apt update && sudo apt install cmake
> ```

> GoogleTest はサブモジュールとして追加されているため、本ライブラリを `--recursive` オプションを付けずクローンした場合追加されません。サブモジュールをクローンするには次のコマンドを実行します。
>
> ```sh
> git submodule update --init --recursive
> ```
