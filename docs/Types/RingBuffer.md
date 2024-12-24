# RingBuffer

疑似可変長リングバッファです。配列の最初と最後の要素を疑似的に繋げ、リング上にした FIFO バッファです。

```cpp
template <typename T, size_t Capacity>
class RingBuffer
{
    T      data[Capacity];
    size_t head;
    size_t tail;
    size_t size;
};
```

## 構築

テンプレート引数に要素の型、最大要素数を指定します。最大要素数と、サイズは意味が異なるので注意してください。

```cpp
template <typename T, size_t Capacity>
class RingBuffer
{
    T data[Capacity];
    size_t head;
    size_t tail;
    size_t size;
};
```

## 構築

```cpp
Udon::RingBuffer<int, 256> ring;                      // 空
Udon::RingBuffer<int, 256> ring(100);                 // サイズから構築
Udon::RingBuffer<int, 256> ring(100, 0);              // サイズ、初期値から構築
Udon::RingBuffer<int, 256> ring{ 1, 2, 3, 4, 5, 6 };  // initializer_list から構築
```

## 要素の追加、削除

```cpp
Udon::RingBuffer<int, 256> ring{ 1, 2, 3, 4, 5, 6 };

ring.push(7);
// ring: { 1, 2, 3, 4, 5, 6, 7    }

ring.push(8);
// ring: { 1, 2, 3, 4, 5, 6, 7, 8 }

ring.pop();
// ring: {    2, 3, 4, 5, 6, 7, 8 }

ring.pop();
// ring: {       3, 4, 5, 6, 7, 8 }
```

## イテレーション

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
