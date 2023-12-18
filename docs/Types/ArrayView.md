# ArrayView

すでにメモリが確保されている領域を参照するクラスです。内部に配列の先頭を指すポインタと、要素数を持っています。

C# の `Span<T>`、C++20 の `std::span<T>` と同じ機能です。

メンバの配置は次のようになっています。

```cpp
template <typename T>
struct ArrayView
{
    T* m_begin;
    size_t size;
};
```

## 構築

テンプレート引数に要素の型を指定します。

```cpp
Udon::ArrayView<int> view;
```

配列の参照から

```cpp
int array[100];
Udon::ArrayView<int> view{ array };
```

アロケート先を指すポインタと要素数から

```cpp
size_t size = 100;
int* array = new int[size];
Udon::ArrayView<int> view{ array, size };
```

`std::vector<T>` から (上と同じくポインタ、要素数から構築)

```cpp
std::vector<int> vector{ 0, 1, 2, 3, 4, 5 };
Udon::ArrayView<int> view{ vector.data(), vector.size() };
```

## ビューから新しいビューを生成する



## イテレーション

```cpp
for (auto&& it : view)
{
    it = 100;
}
```

```cpp
view[2] = 100;  // ランダムアクセスOK
```

> 使用可能なイテレーターは `iterator` `const_iterator` `reverse_iterator` `const_reverse_iterator` です。
