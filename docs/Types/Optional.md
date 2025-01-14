# Optional 型

Optional 型は有効な値を持つか、もしくは無効な値かを表す型です。

`std::optional` 型と同義ですが、C++17 からでしか使えないため `Udon:Optional` 型を実装しています。大まか次のように実装されています。テンプレート引数に渡された型のオブジェクト(値)を持ちます。

```cpp
template <typename T>
class Optional
{
    T    value;     // 値
    bool hasValue;  // true: 有効値  false: 無効値
};
```

次のように戻り値にエラー情報を付与したい場合に用います。

```cpp
Udon::Optional<int> division(int l, int r)
{
    if (r == 0)
        return Udon::nullopt;  // 無効値
    else
        return l / r;          // 有効値
}
```

## 個別インクルード

```cpp
#include <Udon/Types/Optional.hpp>
```

## 有/無効値の判定

```cpp
if (const Udon::Optional<int> result = division(100, 0))
{
    // 有効値
}
else
{
    // 無効値
}
```

有効値かどうかは `hasValue()` `operator bool` メンバ関数によって取得できます。

```cpp
const Udon::Optional<int> result = division(100, 0);

bool hasValue = result.hasValue();          // hasValue == false
bool hasValue = static_cast<bool>(result);  // hasValue == false
```

if 文の判定式にオブジェクトを渡すと、bool 型への明示的なキャストとしてコンパイルされ、`operator bool` が呼び出されます。

```cpp
const Udon::Optional<int> result = division(100, 0);

if (result)
{
    // 有効値
}
else
{
    // 無効値
}
```

一般的には最初の例のように書きます。変数のスコープを if 文に閉じ込められるので、よりスマートです。

## 値の参照

値は `operator *` `value()` によって参照できます。

無効値の場合、値の取得はしてはならず、未定義動作となります。例外の送出できる環境では、例外オブジェクト `Udon::BadOptionalAccess` がスローされます。

```cpp
if (const Udon::Optional<int> result = division(100, 10))
{
    int value = result.value();  // value == 10
}
```

`Optional::operator *` はポインタでの `*` を模倣して定義されています。ポインタでの `*` 演算子はポインタ先の参照を返すのに対し、`Optional::operator *` が持つ値オブジェクトへの参照を返します。

```cpp
if (const Udon::Optional<int> result = division(100, 10))
{
    int value = *result;  // value == 10
}
```

Optional が 構造体オブジェクトを持つ場合は `operator->` によって構造体のメンバを直接参照できます。こちらもポインタのアロー演算子の模倣です。

```cpp
struct Vec2
{
    double x;
    double y;
};
```

```cpp
Udon::Optional<Vec2> divisionVector(const Vec2& l, double r)
{
    if (r == 0)
        return Udon::nullopt;
    else
        return { l.x / r, r.y / r };
}

// ... 略

if (const Udon::Optional<Vec2> result = divisionVector({ 100, 200 }, 5))
{
    double x = result->x;
}
```

## 有効値のセット

通常の変数と同じように扱えます。

初期化時にセット

```cpp
Udon::Optional<int> value = 100;

if (value)
{
    // exec this statement
}
```

代入時にセット

```cpp
Udon::Optional<int> value;

value = 100;

if (value)
{
    // exec this statement
}
```

## 無効値のセット

`Udon::nullopt` オブジェクトを初期化、代入時にセットすると無効値となります。

```cpp
Udon::Optional<int> nullOptional = Udon::nullopt;

if (nullOptional)
{
}
else
{
    // exec this statement
}
```

また初期値を渡さずインスタンス化した際も無効値となります。

```cpp
Udon::Optional<int> nullOptional;
```

既に有効値を持つ場合、`reset` メンバ関数で無効値にできます。

```cpp
Udon::Optional<int> value = 100;

value.reset();

if (nullOptional)
{
}
else
{
    // exec this statement
}
```

## 本ライブラリでの使われ方

本ライブラリでは Optional 型を通信受信時のエラーチェックに用いています。

受信用クラスには受信したオブジェクトを返す `getMessage()` メンバ関数がありますが、この関数は受信した `T` 型オブジェクトを `Optional<T>` で返します。

受信エラーの際は無効値を返すため、エラー処理を次のように書けます。

```cpp
ReaderClass<Udon::Message::Motor> reader;

void loop()
{
    if (const Udon::Optional<Udon::Message::Motor> message = reader.getMessage())
    {
        motor.move(message->power);
    }
    else
    {
        // receive failed
    }
}
```

`auto` 型で受け取ると短く書けます。

```cpp
if (const auto message = reader.getMessage())
{
    motor.move(message->power);
}
else
{
    // receive failed
}
```
