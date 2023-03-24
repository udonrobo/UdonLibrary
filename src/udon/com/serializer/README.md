# Serializer

Serializer は構造体など任意の型のオブジェクトをバイト列に変換します。

メモリアライメントを意識せずに安全に変換できます。

STL などの外部ライブラリで定義された型についても変換用関数を実装することで変換できます。

エンディアン変換を行えるため、リトルエンディアン(Windows 等)、ビッグエンディアン(MacOS 等)の間でも通信できます。

## Data

-   ファイル名

    `Serializer.hpp`

-   依存ファイル

    STL: `vector` `algorithm`

    STL が使用できない環境: `ArduinoSTL.h` `vector` `algorithm`

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

    ユーザー定義型をシリアライズ、デシリアライするには変換用の関数が実装されている必要があります。

    例えば x,y のデータを持つ 2 次元ベクトルの場合、変換用の挿入演算子、抽出演算子を次のようにオーバーロードします。`operator<<` はシリアライズ時に使用され、`operator>>` はデシリアライズ時に使用されます。

    ```cpp
    struct Vec2
    {

    	double x;
    	double y;

    	friend udon::Serializer& operator<<(udon::Serializer& builder, const Vec2& rhs)
    	{
    		return builder << rhs.x << rhs.y;
    	}
    	friend udon::Serializer& operator>>(udon::Serializer& builder, Vec2& rhs)
    	{
    		return builder >> rhs.x >> rhs.y;
    	}

    };
    ```

    ライブラリから型が提供されている場合、非侵入な変換用関数にすることもできます。

    ```cpp

    /// @brief ライブラリが提供している型
    struct Vec2
    {
    	double x;
    	double y;
    };

    udon::Serializer& operator<<(udon::Serializer& builder, const Vec2& rhs)
    {
    	return builder << rhs.x << rhs.y;
    }
    udon::Serializer& operator>>(udon::Serializer& builder, Vec2& rhs)
    {
    	return builder >> rhs.x >> rhs.y;
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

-   高速化

    シリアライズ時に挿入演算子が呼ばれるたび `Serializer` クラスのバッファーにバイト列を挿入する処理が行われます。この処理の都度、バッファーが再アロケーションを行う可能性があります。再アロケーションはほかの処理に比べ非常に重たいです。`Serializer::reserve()` を使い、あらかじめメモリ領域を確保することで再アロケーションを防ぐことができます。

    ```cpp
    struct Vec2
    {

    	double x;
    	double y;

    	friend udon::Serializer& operator<<(udon::Serializer& builder, const Vec2& rhs)
    	{
    		builder.reserve(sizeof rhs.x + sizeof rhs.y);  // メモリアラインを含めたサイズになるため sizeof rhs としてはいけません
    		return builder << rhs.x << rhs.y;
    	}
    	friend udon::Serializer& operator>>(udon::Serializer& builder, Vec2& rhs)
    	{
    		return builder >> rhs.x >> rhs.y;
    	}

    };
    ```

## API

-   `std::vector<uint8_t> Pack(const Ty& object, bool reverseEndian = false)`

    > オブジェクトをバイト列にシリアライズする
    >
    > double型, float型の場合、 `udon::float32_t` 型にキャストされシリアライズされます。
    >
    > `object` シリアライズされるオブジェクト
    >
    > `reverseEndian` エンディアンの変換を行うか
    >
    > `return` シリアライズ後のバイト列

-   `Ty Unpack<Ty>(const std::vector<uint8_t>& buffer, bool reverseEndian = false)`

    > バイト列からオブジェクトを構築する
    >
    > double型, float型の場合、`udon::float32_t` 型にデシリアライズ後、キャストされ返却されます。
    >
    > `Ty` 構築するオブジェクトの型
    >
    > `buffer` デシリアライズされるバイト列
    >
    > `reverseEndian` エンディアンの変換を行うか
    >
    > `return` 構築されたオブジェクト

-   `ReverseEndian`

    > エンディアン変換用マニピュレータ
    >
    > エンディアン変換は、シリアライズ後、デシリアライズ前に行うこと
    >
    > ```cpp
    > // シリアライズ時
    > udon::Serializer builder;
    > builder << object << ReverseEndian;
    >
    > // デシリアライズ時
    > udon::Serializer builder { buffer };
    > builder >> ReverseEndian >> object;

-   `class Serializer`

    -   `Serializer()`

        > デフォルトコンストラクタ
        >
        > シリアライズ時に使用

    -   `Serializer(const std::vector<uint8_t>& buffer)`

        > コンストラクタ
        >
        > `buffer` デシリアライズされるバッファ
        >
        > デシリアライズ時に使用

    -   `void reserve(size_t size)`

        > バッファの capacity を変更する
        >
        > `size` 変更するサイズ

    -   `std::vector<uint8_t> data()`

        > バッファを取得する
        >
        > `return` バッファのコピー

    -   `operator<<(scalar rhs)`

        > シリアライザを行う
        >
        > `rhs` シリアライズされるオブジェクト

    -   `operator>>(scalar& rhs)`

        > デシリアライズを行う
        >
        > `rhs` デシリアライズ後のオブジェクトが代入されるオブジェクト