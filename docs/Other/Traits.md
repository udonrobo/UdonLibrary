# トレイト

- [メタ関数](#%E3%83%A1%E3%82%BF%E9%96%A2%E6%95%B0)
- [曖昧なメンバ関数呼び出し](#%E6%9B%96%E6%98%A7%E3%81%AA%E3%83%A1%E3%83%B3%E3%83%90%E9%96%A2%E6%95%B0%E5%91%BC%E3%81%B3%E5%87%BA%E3%81%97)
- [コンセプト](#%E3%82%B3%E3%83%B3%E3%82%BB%E3%83%97%E3%83%88)

## メタ関数

コンパイル時に型情報を取得する関数(クラス)。

`static_assert` と組み合わせることでユーザーに分かりやすいエラーを出したり、コンパイル時処理に利用できます。

> 各メタ関数は `Udon::Traits` 名前空間に属します。

| 名前                          | 説明                                        | ヘッダーファイル                                                                                 |
| ----------------------------- | ------------------------------------------- | ------------------------------------------------------------------------------------------------ |
| `IsWriter<T>`                 | T が送信クラス要件を満たすか調べる          | [Udon/Traits/IsWriter.hpp](./../../src/Udon/Traits/IsWriter.hpp)                                 |
| `IsReader<T>`                 | T が受信クラス要件を満たすか調べる          | [Udon/Traits/IsReader.hpp](./../../src/Udon/Traits/IsReader.hpp)                                 |
| `SerializedSizable<T>`        | T のシリアライズ後サイズを取得可能か調べる  | [Udon/Traits/SerializedSizable.hpp](./../../src/Udon/Traits/SerializedSizable.hpp)               |
| `Accessible<T>`               | T のメンバ変数を列挙可能か調べる            | [Udon/Traits/Accessible.hpp](./../../src/Udon/Traits/Accessible.hpp)                             |
| `Parsable<T>`                 | T が解析可能であるか調べる                  | [Udon/Traits/Parsable.hpp](./../../src/Udon/Traits/Parsable.hpp)                                 |
| `AlwaysFalse<T>`              | 常に `std::false_type` から派生する         | [Udon/Traits/AlwaysFalse.hpp](./../../src/Udon/Traits/AlwaysFalse.hpp)                           |
| `HasMemberFunctionBegin<T>`   | T に `begin` メンバ関数が存在するか調べる   | [Udon/Traits/HasMemberFunctionBegin.hpp](./../../src/Udon/Traits/HasMemberFunctionBegin.hpp)     |
| `HasMemberFunctionUpdate<T>`  | T に `update` メンバ関数が存在するか調べる  | [Udon/Traits/HasMemberFunctionUpdate.hpp](./../../src/Udon/Traits/HasMemberFunctionUpdate.hpp)   |
| `HasMemberFunctionShow<T>`    | T に `show` メンバ関数が存在するか調べる    | [Udon/Traits/HasMemberFunctionShow.hpp](./../../src/Udon/Traits/HasMemberFunctionShow.hpp)       |
| `HasMemberFunctionShowRaw<T>` | T に `showRaw` メンバ関数が存在するか調べる | [Udon/Traits/HasMemberFunctionShowRaw.hpp](./../../src/Udon/Traits/HasMemberFunctionShowRaw.hpp) |

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
struct Parsable
{
    int a;
    double b;
    UDON_PARSABLE(a, b);
}

// T が解析可能である場合この関数が実体化される
template <typename T, typename std::enable_if<Udon::Traits::Parsable<T>::value, std::nullptr_t>::type = nullptr>
void f(const T& rhs)
{
    std::cout << "parsable" << std::endl;
}

int main()
{
    Parsable parsable{};
    f(parsable);
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
template <template <typename> typename Reader>
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

## コンセプト

C++20 で導入されたコンセプトのように型制約を記述できるマクロを提供します。

> [Udon/Traits/Concept.hpp](./../../src/Udon/Traits/Concept.hpp) に定義されています。

| マクロ名                       | コンセプト名             | 説明                                   |
| ------------------------------ | ------------------------ | -------------------------------------- |
| UDON_CONCEPT_BOOL              | Bool                     | bool 型                                |
| UDON_CONCEPT_INTEGRAL          | Integral                 | 整数型                                 |
| UDON_CONCEPT_INTEGRAL_NOT_BOOL | IntegralNotBool          | 整数型かつ bool 型でない               |
| UDON_CONCEPT_FLOATING_POINT    | FloatingPoint            | 浮動小数点型                           |
| UDON_CONCEPT_ATOMIC            | Atomic                   | アトミック型                           |
| UDON_CONCEPT_SCALAR            | Scalar                   | スカラ型                               |
| UDON_CONCEPT_ARRAY             | Array                    | 配列型                                 |
| UDON_CONCEPT_ACCESSIBLE        | Accessible               | メンバ変数を列挙可能な型               |
| UDON_CONCEPT_CAPACITABLE       | SerializedSizable        | シリアライズ後サイズを取得可能な型     |
| UDON_CONCEPT_BEGINNABLE        | HasMemberFunctionBegin   | `begin` メンバ関数を呼び出し可能な型   |
| UDON_CONCEPT_UPDATABLE         | HasMemberFunctionUpdate  | `update` メンバ関数を呼び出し可能な型  |
| UDON_CONCEPT_SHOWABLE          | HasMemberFunctionShow    | `show` メンバ関数を呼び出し可能な型    |
| UDON_CONCEPT_SHOW_RAWABLE      | HasMemberFunctionShowRaw | `showRaw` メンバ関数を呼び出し可能な型 |

### オーバーロード解決の優先順位制御

```cpp
UDON_CONCEPT_INTEGRAL
void f(const Integral& rhs)
{
    std::cout << "integer" << std::endl;
}

UDON_CONCEPT_FLOATING_POINT
void f(const FloatingPoint& rhs)
{
    std::cout << "floating point" << std::endl;
}

int main()
{
    f(10);   //> integer
    f(1.0);  //> floating point
}
```
