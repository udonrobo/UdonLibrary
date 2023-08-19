# コンテナ

コンテナとはオブジェクトの集まりを表現するクラスのことです。つまり配列のようなものです。

## ArrayView

すでにメモリが確保されている領域を参照するクラスです。

内部に配列の先頭を指すポインタと、要素数を持っています。

### コンストラクタ

テンプレート引数に要素の型を指定します。

```cpp
Udon::ArrayView<int> view;
```

配列の参照から構築する

```cpp
int array[100];
Udon::ArrayView<int> view{ array };
```

ポインタと要素数から構築する

```cpp
size_t size = 100;
int* array = new int[size];
Udon::ArrayView<int> view{ array, size };
```

### イテレータ

イテレータを提供するため範囲 for 文を使用できます。ランダムアクセス可能です。

```cpp
for (auto&& it : view) {
    it = 100;
}
```

## RingBuffer

配列の最初と最後の要素を疑似的に繋げ、リング上にした FIFO バッファ。

要素の個数は可変です(疑似可変長)

### コンストラクタ

第一テンプレート引数に要素の型を指定します。

第二引数にバッファの容量を指定します。

```cpp
Udon::RingBuffer<int, 256> buffer;
```

> この場合、初期サイズは 0 、容量は 256 です。
>
> `buffer.size() == 0`
>
> `buffer.capacity() == 256`

サイズから構築する。

```cpp
Udon::RingBuffer<int, 256> buffer{ 100 };
```

サイズ、初期値から構築する

```cpp
Udon::RingBuffer<int, 256> buffer{ 100, 0 };
```

initializer_list から構築する。

```cpp
Udon::RingBuffer<int, 256> buffer{ 1, 2, 3, 4, 5, 6 };
```


