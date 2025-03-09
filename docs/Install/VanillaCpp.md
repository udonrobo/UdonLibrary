# バニラ C++ 環境

バニラ C++ の環境ではヘッダーオンリーライブラリとして振舞います。インクルードディレクトリの指定、STL の使用が可能な場合使用できます。バニラ C++ 環境では、マイコン用の機能は使用できません。

本ライブラリをクローン後、`UdonLibrary/src` ディレクトリをインクルードディレクトリとして指定してください。

## 構成例

ディレクトリ構成

```sh
Udon/
├── main.cpp
└── lib/
    └── UdonLibrary/  # 本ライブラリ
```

main.cpp

```cpp
#include <Udon.hpp>

int main()
{
    Udon::Show("hello world");
}
```

build run (gcc)

```sh
# in Udon/ directory
g++ -o main main.cpp -I ./lib/UdonLibrary/src
./main
```

build run (clang)

```sh
# in Udon/ directory
clang++ -o main main.cpp -I ./lib/UdonLibrary/src
./main
```
