# Serializer

このファイルには、構造体など任意の型のオブジェクトをバイト列に変換する `Serializer` クラス、バイト列からオブジェクトに変換する `Deserializer` クラスが含まれます。また、オブジェクトのメモリアラインを除去したサイズを求める `PackedSizeChecker`も含まれます。

メモリアライメントを意識せずに安全に変換できます。

STL などの外部ライブラリで定義された型についても変換用関数を実装することで変換できます。

エンディアン変換を行えるため、リトルエンディアン(Windows 等)、ビッグエンディアン(MacOS 等)の間でも通信できます。

## Data

-   ファイル名

    `Serializer.hpp`

-   依存ファイル

    STL: `vector` `algorithm`

-   名前空間

    `udon`

## Usage

-   基本

    オブジェクトは次のように `Pack` 関数を使いシリアライズします。この関数はシリアライズ後のバイト列を `std::vector<uint8_t>` で返します。

    ```cpp
    const double object = 1.234;
    const std::vector<uint8_t> packed = udon::Pack(object);
    ```

    シリアライズされたオブジェクトをデシリアライズするには次のように `Unpack` 関数を使います。テンプレートパラメーターにオブジェクトの型を指定する必要があります。ここで型を間違えてしまうと正しく復元されません。

    ```cpp
    const double unpacked = udon::Unpack<double>(packed);
    ```

-   ユーザー定義型の変換

    ユーザー定義型をシリアライズ、デシリアライするにはメンバイテレーション用演算子 `operator|` がオーバーロードされている必要があります。

    ```cpp
    struct Vec2
    {

        double x;
        double y;

        template<class MIterator>
        friend MIterator& operator|(MIterator& mit, Vec2& rhs)
        {
            return mit
                | rhs.x
                | rhs.y;
        }

    };
    ```

    ライブラリから型が提供されている場合、非侵入なグローバル関数にすることもできます。

    ```cpp

    /// @brief ライブラリが提供している型
    struct Vec2
    {
    	double x;
    	double y;
    };

    template<class MIterator>
    MIterator& operator|(MIterator& mit, Vec2& rhs)
    {
        return mit
            | rhs.x
            | rhs.y;
    }
    ```

    ```cpp
    const Vec2 object{ 100, 200 };

    // シリアライズ
    const std::vector<uint8_t> packed = udon::Pack(object);

    // デシリアライズ
    const Vec2 unpacked = udon::Unpack<Vec2>(packed);

    // 出力
    Serial.print(unpacked.x), Serial.print(unpacked.y);
    ```

## API

-   グローバル関数

    -   `std::vector<uint8_t> udon::Pack(const Ty& object, bool reverseEndian = false)`

        > オブジェクトをバイト列にシリアライズする
        >
        > double 型, float 型の場合、 `udon::float32_t` 型にキャストされシリアライズされます。
        >
        > `object` シリアライズされるオブジェクト
        >
        > `reverseEndian` エンディアンの変換を行うか
        >
        > `return` シリアライズ後のバイト列

    -   `Ty udon::Unpack<Ty>(const std::vector<uint8_t>& buffer, bool reverseEndian = false)`

        > バイト列からオブジェクトを構築する
        >
        > double 型, float 型の場合、`udon::float32_t` 型にデシリアライズ後、キャストされ返却されます。
        >
        > `Ty` 構築するオブジェクトの型
        >
        > `buffer` デシリアライズされるバイト列
        >
        > `reverseEndian` エンディアンの変換を行うか
        >
        > `return` 構築されたオブジェクト

-   マニピュレータ

    -   `udon::ReverseEndian`

        > エンディアン変換用マニピュレータ
        >
        > エンディアン変換は、シリアライズ後、デシリアライズ前に行うこと
        >
        > ```cpp
        > // シリアライズ時
        > udon::Serializer builder;
        > builder | object | udon::ReverseEndian;
        >
        > // デシリアライズ時
        > udon::Deserializer builder { buffer };
        > builder | udon::ReverseEndian | object;
        > ```

-   クラス

    -   `udon::Serializer`

        オブジェクトからバイト列に変換します。

        -   `Serializer()`

            > デフォルトコンストラクタ

        -   `void reserve(size_t size)`

            > バッファの capacity を変更する
            >
            > `size` 変更するサイズ

        -   `std::vector<uint8_t> data()`

            > バッファを取得する
            >
            > `return` バッファのコピー

        -   `Serializer& operator|(scalar rhs)`

            > スカラ型のシリアライザを行う
            >
            > `rhs` シリアライズされるオブジェクト

        -   `Serializer& operator|(Ty(&rhs)[N])`

            > 配列型のシリアライズを行う
            >
            > 配列のシリアライズは再帰的に行われるため、多次元配列であってもシリアライズ可能です。
            >
            > 配列の要素がシリアライズ可能である場合のみ、シリアライズ可能です。
            >
            > `rhs` シリアライズされる配列オブジェクト

    -   `udon::Deserializer`

        バイト列からオブジェクトに変換します。

        -   `Deserializer(const std::vector<uint8_t>& buffer)`

            > コンストラクタ
            >
            > `buffer` デシリアライズされるバッファ

        -   `Deserializer& operator|(scalar& rhs)`

            > スカラ型のデシリアライズを行う
            >
            > `rhs` デシリアライズ後のオブジェクトが代入されるオブジェクト

        -   `Deserializer& operator|(Ty(&rhs)[N])`

            > 配列型のデシリアライズを行う
            >
            > 配列のデシリアライズは再帰的に行われるため、多次元配列であってもデシリアライズ可能です。
            >
            > 配列の要素がデシリアライズ可能である場合のみ、デシリアライズ可能です。
            >
            > `rhs` デシリアライズ後のオブジェクトが代入される配列オブジェクト

    -   `udon::PackedSizeChecker`

        オブジェクトのメモリアラインを除去したサイズを求めます。

        -   `PackedSizeChecker`

            > デフォルトコンストラクタ

        -   `PackedSizeChecker& operator|(scalar& rhs)`

            > スカラ型のサイズを求める

        -   `PackedSizeChecker& operator|(Ty(&rhs)[N])`

            > 配列型のサイズを再帰的に求める

        -   `size_t size()`

            > メモリアラインを除去したサイズを取得する
