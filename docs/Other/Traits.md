# トレイト

## メタ関数

コンパイル時に型情報を取得する関数(クラス)。

`static_assert` と組み合わせることでユーザーに分かりやすいエラーを出したり、コンパイル時処理に利用できます。

`std::true_type`、`std::false_type` のいずれかを継承するため、`value` メンバ変数を使用して、判定結果をコンパイル時に取得できます。

> 各メタ関数は `Udon::Traits` 名前空間に属します。

| 名前                          | 説明                                                                                             | ヘッダーファイル                                                                         |
| ----------------------------- | ------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------- |
| `IsWriter<T>`                 | T が送信クラス要件を満たす場合 `std::true_type` 、それ以外は `std::false_type` を継承。          | [Udon/Traits/ReaderWriterTraits.hpp](./../../src/Udon/Traits/ReaderWriterTraits.hpp)     |
| `IsReader<T>`                 | T が受信クラス要件を満たす場合 `std::true_type` 、それ以外は `std::false_type` を継承。          | [Udon/Traits/ReaderWriterTraits.hpp](./../../src/Udon/Traits/ReaderWriterTraits.hpp)     |
| `IsSerializable<T>`           | T がシリアライズ可能な場合 `std::true_type` 、それ以外は `std::false_type` を継承。              | [Udon/Serializer/SerializerTraits.hpp](./../../src/Udon/Serializer/SerializerTraits.hpp) |
| `AlwaysFalse<T>`              | 常に `std::false_type` を継承。(遅延 static_assert)                                              | [Udon/Traits/AlwaysFalse.hpp](./../../src/Udon/Traits/AlwaysFalse.hpp)                   |
| `HasMemberFunctionBegin<T>`   | T に `begin` メンバ関数が存在する場合 `std::true_type` 、それ以外は `std::false_type` を継承。   | [Udon/Traits/HasMemberFunction.hpp](./../../src/Udon/Traits/HasMemberFunction.hpp)       |
| `HasMemberFunctionUpdate<T>`  | T に `update` メンバ関数が存在する場合 `std::true_type` 、それ以外は `std::false_type` を継承。  | [Udon/Traits/HasMemberFunction.hpp](./../../src/Udon/Traits/HasMemberFunction.hpp)       |
| `HasMemberFunctionShow<T>`    | T に `show` メンバ関数が存在する場合 `std::true_type` 、それ以外は `std::false_type` を継承。    | [Udon/Traits/HasMemberFunction.hpp](./../../src/Udon/Traits/HasMemberFunction.hpp)       |
| `HasMemberFunctionShowRaw<T>` | T に `showRaw` メンバ関数が存在する場合 `std::true_type` 、それ以外は `std::false_type` を継承。 | [Udon/Traits/HasMemberFunction.hpp](./../../src/Udon/Traits/HasMemberFunction.hpp)       |

### 送信クラス要件

- `MessageType` というメンバ型名を持つ
- `setMessage()` メンバ関数を持ち、戻り値が `void`、引数型が `const MessageType&` である

```cpp
// 送信クラス要件を満たすクラス
template <typename Message>
class Writer
{
public:
    using MessageType = Message;
    void setMessage(const MessageType& message)
    {
    }
};

// 送信クラス要件を満たしていないクラス
template <typename>
class NotWriter {};

int main()
{
    static_assert(Udon::Traits::IsWriter<Writer>::value, "");  // OK
    // static_assert(Udon::Traits::IsWriter<NotWriter>::value, "");  // static_assert failed
}
```

### 受信クラス要件

- `MessageType` というメンバ型名を持つ
- `getMessage() const` メンバ関数を持ち、戻り値が `Udon::Optional<MessageType>`、引数が存在しない

```cpp
// 受信クラス要件を満たすクラス
template <typename Message>
class Reader
{
public:
    using MessageType = Message;
    Udon::Optional<MessageType> getMessage() const
    {
    }
};

// 受信クラス要件を満たしていないクラス
template <typename>
class NotReader {};

int main()
{
    static_assert(Udon::Traits::IsReader<Reader>::value, "");  // OK
    // static_assert(Udon::Traits::IsWriter<NotWriter>::value, "");  // static_assert failed
}
```

### オーバーロード解決の優先順位制御

```cpp
struct Serializable
{
    int a;
    double b;
    UDON_ENUMERABLE(a, b);
}

// T がシリアライズ可能である場合この関数が実体化される
template <typename T, typename std::enable_if<Udon::Traits::IsSerializable<T>::value, std::nullptr_t>::type = nullptr>
void f(const T& rhs)
{
    std::cout << "serializable" << std::endl;
}

int main()
{
    Serializable serializable{};
    f(serializable);
}
```

### `AlwaysFalse<T>`

`AlwaysFalse<T>` を使用することで、テンプレート関数、クラスが実体化されたときに限り `static_assert` を失敗させることができる。

> static_assert(false, "") は常に失敗してしまうので、失敗を実体化まで遅延させる。

```cpp
template <typename T, typename std::enable_if<std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
void f(const T& rhs)
{
    std::cout << "integer" << std::endl;
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
void f(const T& rhs)
{
    static_assert(Udon::Traits::AlwaysFalse<T>::value, "Floating-point types are not supported");
}

int main()
{
    f(10);  // OK
    // f(1.0);  // static_assert failed
}
```

## 曖昧なメンバ関数呼び出し

特定のメンバ関数が存在する場合呼び出し、存在しない場合何もしない関数です。この関数を使用することで、特定のメンバ関数の有無に関係なく一様な記述が可能になります。

> 各関数は `Udon::Traits` 名前空間に属します。
>
> メタ関数 `HasMemberFunction~~` が定義されているヘッダーファイルに定義されています。

| 名前                      | 説明                                           | 戻り値型 | 引数型 |
| ------------------------- | ---------------------------------------------- | -------- | ------ |
| `MaybeInvokeBegin(rhs)`   | `rhs.begin()` を呼び出せる場合呼び出す         | 不定     | void   |
| `MaybeInvokeUpdate(rhs)`  | `rhs.update()` を呼び出せる場合呼び出す        | 不定     | void   |
| `MaybeInvokeShow(rhs)`    | `rhs.show() const` を呼び出せる場合呼び出す    | 不定     | void   |
| `MaybeInvokeShowRaw(rhs)` | `rhs.showRaw() const` を呼び出せる場合呼び出す | 不定     | void   |

### 基本

```cpp
class Updatable
{
public:
    void update()
    {
        std::cout << "Updatable::update()" << std::endl;
    }
};

class Unupdatable {};

int main()
{
    Updatable updatable;
    Udon::Traits::MaybeInvokeUpdate(updatable);  //> Updatable::update()

    Unupdatable unupdatable;
    Udon::Traits::MaybeInvokeUpdate(unupdatable);  //>
}
```

### 実用例

```cpp
// updateを持つ受信クラス
template <typename Message>
class ReaderA
{
public:
    void update()
    {
        std::cout << "ReaderA::update()" << std::endl;
    }
    // ...
};

// updateを持たない受信クラス
template <typename Message>
class ReaderB
{
    // ...
};

// 受信クラス経由でセンサーの情報を取得するクラス
template <template <typename> class Reader>
class Sensor
{
    Reader<int> reader;
public:
    void update()
    {
        Udon::Traits::MaybeInvokeUpdate(reader);
        // ...
    }
};

int main()
{
    Sensor<ReaderA> sensorA;
    sensorA.update();  //> ReaderA::update()

    Sensor<ReaderB> sensorB;
    sensorB.update();  //>
}
```
