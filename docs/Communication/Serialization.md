# シリアライザー

```cpp
#include <Udon/Com/Serialization.hpp>
```

オブジェクトのバイト列化、バイト列からオブジェクトへの復元を行います。

> 使用例
>
> ```mermaid
> flowchart LR
>     subgraph マイコン
>     まいこんおぶ[オブジェクト] --シリアライズ--> a[バイト列]
>     end
>     subgraph パソコン
>     a[バイト列] --> バイト列 --デシリアライズ--> ぱそこんおぶ[オブジェクト]
>     end
> ```

## シリアライズ

シリアライズしたいオブジェクトのデータ構造を構造体、クラスを用いて定義します。`UDON_PARSABLE` マクロにメンバ変数を登録することで、メンバ変数を解析できるようになり、シリアライズできるようになります。

`Udon::Pack(object)` の引数にオブジェクトを渡すことでバイト列 `std::vector<uint8_t>` が返されます。

```cpp
struct Vec2
{
    double x;
    double y;
    UDON_PARSABLE(x, y);
};

void setup() {}

void loop()
{
    Vec2 v{ 2.5, 3.4 };
    const std::vector<uint8_t> packed = Udon::Pack(v);
}
```

## デシリアライズ

バイト列をオブジェクトに復元するには `Udon::Unpack<T>(byte[])` を使用します。 `Udon::Unpack` はデシリアライズされたオブジェクトを `Udon::Optional` でラップして返します。
`Optional`が返ることでデシリアライズできたかどうかの判定ができます。`Optional` は `operator bool()` メンバを持っているため、次のように if 文で判定可能です。

> バイト列末端バイトに挿入されているチェックサムと、バイト列から求めたチェックサムが異なる場合、デシリアライズが失敗します。
> チェックサムの整合が取れない場合、データが破損しています。

```cpp
void loop()
{
    // packed にはシリアライズされたバイト列が入っている
    if (const Udon::Optional<Vec2> unpacked = Udon::Unpack<Vec2>(packed))
    {
        Serial.print(unpacked->x);  Serial.print('\t');
        Serial.print(unpacked->y);  Serial.print('\n');
    }
    else
    {
        Serial.println("Unpack failed!");  // データ破損
    }
}
```

## 既に定義されている型をシリアライズ、デシリアライズ

**クローバル空間** に `Capacity(const T&)` `voidAccessor<Acc>(Acc&, T&)` 関数を次のように定義することで、ライブラリ等で既に定義されている型をシリアライズできます。

```cpp
// ↓ 外部ライブラリ内に定義されている Vec2
struct Vec2
{
    double x;
    double y;
}

// シリアライズ後のビット数をコンパイル時に取得する関数
constexpr size_t Capacity(const Vec2& rhs)
{
    return Udon::CapacityBits(rhs.x, rhs.y);
}

// シリアライザ、デシリアライザが使用するメンバ変数解析用関数
template <typename Acc>
void Accessor(Acc& acc, Vec2& rhs)
{
    acc(rhs.x, rhs.y);
};
```

> `Capacity` は `Udon::CapacityWithChecksum` から呼び出されます。`Udon::CapacityBits(...)` はシリアライズ後のビット数をコンパイル時に取得できる関数です。

## API

- `Udon::Pack`

  オブジェクトをシリアライズします

  Pack 関数は次のオーバーロードが定義されています。

  ```cpp
  std::vector<uint8_t> Pack(const T& object)

  bool Pack(const T& object, uint8_t* buffer, size_t size)

  bool Pack(const T& object, uint8_t (&array)[N])
  ```

- `Udon::Unpack<T>`

  バイト列をオブジェクトにデシリアライズします

  Unpack 関数は次のオーバーロードが定義されています。

  ```cpp
  template <typename T>
  Udon::Optional<T> Unpack(const std::vector<uint8_t>& buffer)

  template <typename T>
  Udon::Optional<T> Unpack(const uint8_t* buffer, size_t size)

  template <typename T>
  Udon::Optional<T> Unpack(const uint8_t (&array)[N])
  ```

- `Udon::CanUnpack`

  デシリアライズできるかを確認します。(チェックサム確認)

  ```cpp
  bool CanUnpack(const std::vector<uint8_t>& buffer)

  bool CanUnpack(const uint8_t* buffer, size_t size)

  bool CanUnpack(const uint8_t (&array)[N])
  ```

- `Udon::CapacityWithChecksum<T>`

  T 型オブジェクトシリアライズ後のバイト列のバイトサイズを取得します。

  コンパイル時にサイズを取得可能なため、バッファの大きさを静的に指定するときなどにも使えます。

  ```cpp
  uint8_t buffer[Udon::CapacityWithChecksum<Vec2>()];
  ```

- `Udon::CapacityBits(...)`

  オブジェクトをシリアライズした際のバイト列のビットサイズを取得します。(bool 型 を 1bit としてカウントするため)

  `Capacity` 関数を外部に定義するときに使用します。引数は可変長引数です。

## 詳細

プリプロセス時、コンパイル時、実行時の各処理の詳細。Vec2 型を例にします。

### プリプロセス時

`UDON_PARSABLE` マクロの展開を行います。

`UDON_PARSABLE` 内部で定義されるメンバ関数を使用してメンバ変数の解析を行いシリアライズ処理を行います。

```cpp
struct Vec2
{
    double x;
    double y;
    UDON_PARSABLE(x, y);
};
```

↓

```cpp
struct Vec2
{
    double x;
    double y;

    constexpr bool parsable() const
    {
        return Udon::Traits::IsMemberParsable(x, y);
    }

    constexpr size_t capacityBits() const
    {
        return Udon::CapacityBits(x, y);
    }

    template <typename Acc>
    void accessor(Acc& acc)
    {
        acc(x, y);
    };
};
```

### コンパイル時

0. `Udon::Pack<T>` 呼び出し箇所発見

1. `Udon::Pack<Vec2>` インスタンス化

2. `Udon::Traits::IsParsable<Vec2>` インスタンス化

   1. `parsable()` 関数呼び出し
   2. `Udon::Traits::IsMemberParsable(x, y)` 関数が引数を展開し解析可能か判定。(再帰的)

3. `Udon::CapacityWithChecksum<T>()` 呼び出し箇所発見

   1. `capacityBits()` 関数呼び出し
   2. `Udon::CapacityBits(Args...)` 関数が引数を展開し、シリアライズ後のバイト数を求める。(再帰的)
   3. 呼び出し箇所定数化

4. `Udon::Serializer` インスタンス化箇所発見

   1. `accessor<Udon::Serializer>(acc)` インスタンス化
   2. `Udon::Serializer::operator(Args...)` 呼び出し箇所発見( acc(x, y) )
   3. `operator(Args...)` へ渡された引数展開
   4. `Udon::Serializer::serialize<double>` インスタンス化 (x 用)
   5. `Udon::Serializer::serialize<double>` インスタンス化 (y 用)

### 実行時

    todo