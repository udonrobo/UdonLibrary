# シリアライズ デシリアライズ

## Description

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

## Usage

### シリアライズ

シリアライズしたいオブジェクトのデータ構造を構造体、クラスを用いて定義します。

`UDON_PARSABLE` マクロにメンバ変数を登録することで、メンバ変数を解析できるようになり、シリアライズ可能になります。

```cpp
struct Vec2
{
    double x;
    double y;
    UDON_PARSABLE(x, y);
};
```

`Udon::Pack(object)` の引数にオブジェクトを渡すことでバイト列 `std::vector<uint8_t>` が返されます。

```cpp
Vec2 v{ 2.5, 3.4 };
const auto packed = Udon::Pack(v);
```

### デシリアライズ

バイト列をオブジェクトに復元するには `Udon::Unpack<T>(byte[])` を使用します。 `Udon::Unpack` はデシリアライズされたオブジェクトを `Udon::Optional` でラップして返します。
`Optional`が返ることでデシリアライズできたかどうかの判定ができます。`Optional` は `operator bool()` メンバを持っているため、次のように if 文で判定可能です。

> バイト列末端バイトに挿入されているチェックサムと、バイト列から求めたチェックサムが異なる場合、デシリアライズが失敗します。
> チェックサムの整合が取れない場合、データが破損しています。

```cpp
if (const Udon::Optional<Vec2> unpacked = Udon::Unpack<Vec2>(packed))
{
    Serial.print(unpacked->x);  Serial.print('\t');
    Serial.print(unpacked->y);  Serial.print('\n');
}
else
{
    Serial.println("Unpack failed!");  // データ破損
}
```

## Specification

- 動作仕様

  浮動小数点型はすべて `Udon::Float32_t` にキャストされシリアライズされます

  > アーキテクチャによって浮動小数点のサイズが異なるため

  bool 型オブジェクトは 1 ビットにシリアライズされます。

  エンディアン変換を自動で行います。

  バイト列末端に 1 バイト CRC8 チェックサムを挿入します。

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

## Details

- `UDON_PARSABLE`

  `UDON_PARSABLE` はシリアライザがメンバ変数を解析できるようにするためのマクロです。次のように展開されます。

  ```cpp
  struct Vec2
  {
      double x;
      double y;

      using IsParsable_tag = void;

      constexpr size_t capacity() const
      {
          return Udon::Capacity(x, y);
      }

      template <typename Acc>
      void accessor(Acc& acc)
      {
          acc(x, y);
      };
  };
  ```

  - `IsParsable_tag`

    > `IsParsable_tag` は型が解析可能かを区別する際に使用します。
    >
    > `Udon::IsParsable<T>` を使用し、型(ここでは Vec2)を解析できるかできないかコンパイル時に判定できるため、`static_assert` によってユーザーに分かりやすくエラーを出すことができます。
    >
    > ```cpp
    > static_assert(Udon::IsParsable<Vec2>::value, "can not serialize");
    > ```

  - `capacity()`

    > `capacity()` はシリアライズ後のバイト列のバイトサイズを求める `CapacityWithChecksum` から呼び出されます。
    >
    > コンパイル時にサイズを取得可能なため、バッファの大きさを静的に指定するときなどにも使えます。
    >
    > ```cpp
    > uint8_t buffer[Udon::CapacityWithChecksum<Vec2>()];
    > ```

  - `accessor()`

    > `accessor()` メンバ変数を解析するときに使用します。解析用クラス(ここでは Udon::Serializer)が呼び出します。
    >
    > テンプレートパラメータ `Acc` には解析用クラスの型が入ります。
    >
    > acc(x, y) とすることで `Udon::Serializer` クラスの可変長 `operator()(...)` が呼ばれ、スカラ型(int, double 等)になるまで再帰的に `operator()()` を呼び出します。このとき、各スカラオブジェクトのシリアル化を行い、`Udon::Serializer` 内部のバッファに挿入していくことでシリアライズを行います。
    >
    > 呼び出し例 (Vec2 の場合)
    >
    > 1. `Udon::Serializer::operator()(Vec2)`
    >
    > 2. `Udon::Serializer::operator()(double, double)`
    >
    > 3. `Udon::Serializer::operator()(double)` `x` のシリアル化
    >
    > 4. `Udon::Serializer::operator()(double)` `y` のシリアル化

- シリアライズ処理順序

  1. `Udon::Pack` 関数を呼び出す

  2. `Udon::Serializer` クラスのオブジェクトが生成される

  3. `accessor()` を使用してメンバを解析、シリアル化、バッファへ挿入

  4. バッファを基にチェックサムを求めバッファの末端に挿入

  5. シリアル化したデータを`Udon::Pack`が返す

- デシリアライズ処理順序

  1. `Udon::Unpack<T>` 関数を呼び出す

  2. 復元したデータを入れる T 型オブジェクトが作成される

  3. `Udon::Deserializer` クラスのオブジェクトが生成される

  4. チェックサムの整合性チェック -> エラーであれば `Udon::Unpack` が `Udon::nullopt` を返し失敗

  5. `accessor()` を使用してメンバを解析、逆シリアル化、T 型オブジェクトへ代入

  6. シリアル化したデータを`Udon::Unpack`が返す
