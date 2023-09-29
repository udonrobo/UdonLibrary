# コンテナ

オブジェクトの集まりを表現する配列のようなクラスです。

- [StaticVector](#staticvector)
- [ArrayView](#arrayview)
- [RingBuffer](#ringbuffer)

## StaticVector

疑似可変長配列。内部に固定長配列を持ち、サイズ変更時にメモリアロケーションが発生しません。

`std::vector<T>` のヒープ領域使用しない版。

### 構築

テンプレート引数に要素の型、最大要素数を指定します。

最大要素数と、サイズは意味が異なるので注意してください

> 最大要素数は現在割り当てられている領域の容量
> サイズは使われている要素数

```cpp
Udon::StaticVector<int, 256> vector;                // 空
Udon::StaticVector<int, 256> vector(10);            // サイズから構築
Udon::StaticVector<int, 256> vector(10, 7);         // サイズ、初期値から構築
Udon::StaticVector<int, 256> vector{ 1, 2, 3, 4 };  // initializer_list から構築
```

### 要素の追加、削除

```cpp
Udon::StaticVector<int, 256> vector;
vector.push_back(123);
vector.push_back(456);
// vector = { 123, 456 };
```

```cpp
Udon::StaticVector<int, 256> vector{ 1, 2, 3, 4 };
vector.pop_back();
// vector = { 1, 2, 3 };
```

### イテレーション

```cpp
for (auto&& it : vector)
{
    it = 100;
}
```

```cpp
vector[2] = 100;  // ランダムアクセスOK
```

> 使用可能なイテレーターは `iterator` `const_iterator` `reverse_iterator` `const_reverse_iterator` です。
>
> `std::vector` と違い、各要素の領域の場所が変わらないのでイテレーター破壊が起きません。

## ArrayView

すでにメモリが確保されている領域を参照するクラスです。

内部に配列の先頭を指すポインタと、要素数を持っています。

### 構築

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

### イテレーション

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

## RingBuffer

疑似可変長リングバッファ。配列の最初と最後の要素を疑似的に繋げ、リング上にした FIFO バッファ。

### 構築

テンプレート引数に要素の型、最大要素数を指定します。

最大要素数と、サイズは意味が異なるので注意してください

> 最大要素数は現在割り当てられている領域の容量
> サイズは使われている要素数

```cpp
Udon::RingBuffer<int, 256> ring;                      // 空
Udon::RingBuffer<int, 256> ring(100);                 // サイズから構築
Udon::RingBuffer<int, 256> ring(100, 0);              // サイズ、初期値から構築
Udon::RingBuffer<int, 256> ring{ 1, 2, 3, 4, 5, 6 };  // initializer_list から構築
```

### 要素の追加、削除

```cpp
Udon::RingBuffer<int, 256> ring{ 1, 2, 3, 4, 5, 6 };  // initializer_list から構築
ring.push(7);
ring.push(8);
// ring = { 1, 2, 3, 4, 5, 6, 7, 8 };
```

### イテレーション

```cpp
for (auto&& it : ring)
{
    it = 100;
}
```

```cpp
ring[2] = 100;  // ランダムアクセスOK
```

> 使用可能なイテレーターは `iterator` `const_iterator` `reverse_iterator` `const_reverse_iterator` です。
