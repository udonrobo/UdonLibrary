# コンテナ

コンテナとはオブジェクトの集まりを表現するクラスのことです。つまり配列のようなものです。

- [StaticVector](#staticvector)
- [ArrayView](#arrayview)
- [RingBuffer](#ringbuffer)

## StaticVector

疑似可変長配列です。内部に固定長配列を持ち、サイズ変更時にメモリアロケーションが発生しません。

`std::vector<T>` と同じメンバを持ちます。

```cpp
template <typename T, size_t Capacity>
struct StaticVector
{
    T m_buffer[Capacity];
    T* m_end;
};
```

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

- 要素の追加、削除

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

- イテレーション

  イテレータを提供するため範囲 for 文を使用できます。

  使用可能なイテレーターは `iterator` `const_iterator` `reverse_iterator` `const_reverse_iterator` です。

  ```cpp
  for (auto&& it : view) {
      it = 100;
  }
  ```

  ランダムアクセス可能です。

  ```cpp
  view[2] = 100;
  ```

## ArrayView

すでにメモリが確保されている領域を参照するクラスです。

内部に配列の先頭を指すポインタと、要素数を持っています。

```cpp
template <typename T>
struct ArrayView
{
    T* m_begin;
    T* m_end;
};
```

### 構築

  テンプレート引数に要素の型を指定します。

  ```cpp
  Udon::ArrayView<int> view;
  ```

  配列の参照から構築する

  ```cpp
  int array[100];
  Udon::ArrayView<int> view{ array };
  ```

  アロケート先を指すポインタと要素数から構築する

  ```cpp
  size_t size = 100;
  int* array = new int[size];
  Udon::ArrayView<int> view{ array, size };
  ```

- イテレーション

  イテレータを提供するため範囲 for 文を使用できます。

  使用可能なイテレーターは `iterator` `const_iterator` `reverse_iterator` `const_reverse_iterator` です。

  ```cpp
  for (auto&& it : view) {
      it = 100;
  }
  ```

  ランダムアクセス可能です。

  ```cpp
  view[2] = 100;
  ```

### 文字列操作

ArrayView はメモリ領域を参照する、領域自体を持たないクラスです。そのため参照する範囲を変更することでビューから再アロケートなしで新たなビューを作成することができます。

- 文字列から構築

  文字列が保存されている領域を参照するビューを作成。

  ```cpp
  int main()
  {
      Udon::ArrayView<const char> stringView = "hogehoge hogehoge hogehoge";
  }
  ```

- `subView`

  指定された範囲から新しいビューを作成。

  ```cpp
  int main()
  {
      Udon::ArrayView<const char> stringView = "hogehoge hogehoge hogehoge";
      //                                     begin^     ^end
      std::cout << stringView.subView(3, 8) << std::endl;
      //> gehoge
  }
  ```

  ```cpp
  int main()
  {
      Udon::ArrayView<const char> stringView = "hogehoge hogehoge hogehoge";
      //                                             begin^               ^end
      std::cout << stringView.subView(10) << std::endl;
      //> ogehoge hogehoge
  }
  ```

- `subViewUntil`

  指定された文字までの文字列からビューを作成。

  ```cpp
  int main()
  {
      Udon::ArrayView<const char> stringView = "hogehoge hogehoge hogehoge";
      //                                   begin^       ^end
      std::cout << stringView.subViewUntil(' ') << std::endl;
      //> hogehoge
  }
  ```

## RingBuffer

配列の最初と最後の要素を疑似的に繋げ、リング上にした FIFO バッファ。

```cpp
template <typename T, size_t Capacity>
class RingBuffer
{
    T m_data[Capacity];
    size_t m_head;
    size_t m_tail;
    size_t m_size;
};
```

### 構築

- 構築

  テンプレート引数に要素の型、最大要素数を指定します。

  最大要素数と、サイズは意味が異なるので注意してください

  > 最大要素数は現在割り当てられている領域の容量
  > サイズは使われている要素数

  ```cpp
  Udon::RingBuffer<int, 256> buffer;
  Udon::RingBuffer<int, 256> buffer(100);                 // サイズから構築
  Udon::RingBuffer<int, 256> buffer(100, 0);              // サイズ、初期値から構築
  Udon::RingBuffer<int, 256> buffer{ 1, 2, 3, 4, 5, 6 };  // initializer_list から構築
  ```

- イテレーション

  イテレータを提供するため範囲 for 文を使用できます。

  使用可能なイテレーターは `iterator` `const_iterator` `reverse_iterator` `const_reverse_iterator` です。

  ```cpp
  for (auto&& it : view) {
      it = 100;
  }
  ```

  ランダムアクセス可能です。

  ```cpp
  view[2] = 100;
  ```
