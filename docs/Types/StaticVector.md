# StaticVector

疑似可変長配列。内部に固定長配列を持ち、サイズ変更時にメモリアロケーションが発生しません。

`std::vector<T>` のヒープ領域を使用しない版です。

メンバの配置は次のようになっています。

```cpp
template <typename T, size_t Capacity>
struct StaticVector
{
    T m_buffer[Capacity];
    size_t size;
};
```

## 構築

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

## 要素の追加、削除

```cpp
Udon::StaticVector<int, 256> vector{ 1, 2, 3, 4 };

vector.push_back(5);
// vector: { 1, 2, 3, 4, 5    }

vector.push_back(6);
// vector: { 1, 2, 3, 4, 5, 6 }

vector.pop_back();
// vector: { 1, 2, 3, 4, 5    }

vector.pop_back();
// vector: { 1, 2, 3, 4       }
```

## イテレーション

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
