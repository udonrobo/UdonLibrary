# バニラ C++ 環境

バニラ C++ の環境ではヘッダーオンリーライブラリとして振舞います。インクルードディレクトリの指定、STL の使用が可能な場合、使用可能です。

本ライブラリをクローン後、`UdonLibrary/src` ディレクトリをインクルードディレクトリとして指定してください。

## 例

### ディレクトリ構造

```sh
Udon/
  | main.cpp
  | lib/
    | UdonLibrary/
```

### main.cpp

```cpp
#include <Udon.hpp>

int main()
{
    Udon::Show("hello world");
}
```

### build run

- gcc

  ```sh
  # in Udon/ directory
  g++ -o main main.cpp -I ./lib/UdonLibrary/src
  ./main
  ```

- clang

  ```sh
  # in Udon/ directory
  clang++ -o main main.cpp -I ./lib/UdonLibrary/src
  ./main
  ```
