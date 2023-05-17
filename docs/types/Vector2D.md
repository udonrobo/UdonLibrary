# Vector2D

二次元ベクトルを表す型

## Description

二次元ベクトルを表現するためのクラスです。x 軸と y 軸の 2 つの要素からなる二次元ベクトルを格納し、ベクトル演算や座標変換などの操作を提供します。

x 軸の右方向を正、y 軸の上方向を正とする直交座標系を使用します。

<img width="40%" src="https://user-images.githubusercontent.com/91818705/230623615-236c3294-c8c6-4685-95de-fa02692b3cc5.png">

## Usage

1.  ファイルインクルード

    ```cpp
    #include <Udon.hpp>
    // or
    #include <udon/types/Vector2D.hpp>
    ```

2.  インスタンス化

    Vector2D クラスは、テンプレート引数を使用して `x` と `y` の要素の型を指定することができます。例えば、要素を `double` 型にしたい場合は次のように記述します。

    ```cpp
    udon::Vector2D<double> vector;
    ```

    また、`udon::Vector2D<double>` のエイリアス(別名)である `Vec2` 型を使用するとより簡潔に記述できます。

    ```cpp
    udon::Vec2 vector;
    ```

    インスタンス化例

    ```cpp
    udon::Vec2 vector;
    udon::Vec2 vector ( 1234.5, 6.789 );
    udon::Vec2 vector { 1234.5, 6.789 };
    udon::Vec2 vector = { 1234.5, 6.789 };
    auto vector = udon::Vec2 { 1234.5, 6.789 };
    ```

-   要素へのアクセス

    `Vector2D` 型のインスタンスには、`x` と `y` という 2 つの要素があります。これらの要素にアクセスするには、メンバ変数へのドット演算子(`.`)を使用します。

    書き込み

    ```cpp
    udon::Vec2 vector { 2, 3 };
    vector.x = 1234.5;
    vector.y = 5678;
    ```

    読み取り

    ```cpp
    const udon::Vec2 vector { 1234.5, 6.789 };
    double x = vector.x;
    double y = vector.y;
    ```

-   ベクトル演算

    `Vector2D` クラスにはベクトル演算を行うための各種演算子の実装がされています。

    加算

    ```cpp
    Vec2 vector1 { 2.0, 3.0 };
    Vec2 vector2 { 1.0, -1.5 };

    Vec2 result = vector1 + vector2;
    // result = { 3.0, 1.5 }
    ```

    減算

    ```cpp
    Vec2 vector1 { 2.0, 3.0 };
    Vec2 vector2 { 1.0, -1.5 };

    Vec2 result = vector1 - vector2;
    // result = { 1.0, 4.5 }
    ```

    スカラ倍

    ```cpp
    Vec2 vector { 2.0, 3.0 };
    double scalar = 1.5;

    Vec2 result = vector * scalar;
    // result = { 3.0, 4.5 }
    ```

-   お便利な関数

    -   ベクトルデータをシリアルモニターに送信する

        `show` メンバ関数を使用します。デバッグ時などに有用です。

        `setup` 関数内で `Serial.begin` を呼び出しシリアル通信が開始されている必要があります。

        ```cpp
        void setup()
        {
        	Serial.begin(115200);

        	Vec2 vector { 3.0, 4.0 };
        	vector.show();

        	//> x: 3.0 [tab] y: 4.0 [tab]
        }
        ```

    -   シリアライズ

        このクラスはシリアライズ(バイト列化)に対応しています。したがって各通信用クラスに渡すことでマイコン同士でベクトルデータのやり取りができます。詳細は各通信用クラスのドキュメントを参照してください。

        ```cpp
        udon::Vec2 vector { 3.0, 4.0 };
        writer.setMessage(vector);
        ```

    -   ベクトルの長さを求める

        `length` メンバ関数を使用します。

        三平方の定理を用いてベクトルの長さを求めます。

        ```cpp
        Vec2 vector { 3.0, 4.0 };

        double length = vector.length();
        // length = 5.0
        ```

    -   他のベクトルとの距離を求める

        `distanceFrom` メンバ関数を使用します。

        ```cpp
        Vec2 vector1 { 1.0, 2.0 };
        Vec2 vector2 { 4.0, 6.0 };

        double distance = vector1.distanceFrom(vector2);  // ベクトル間の距離
        // distance = 5.0
        ```

    -   ベクトルの長さを変更する

        `setLength` メンバ関数を使用します。

        ベクトルの長さ（大きさ）を指定した値に設定します。ベクトルの向きは変わらず、大きさのみが変更されます。

        ```cpp
        Vec2 vector(3.0, 4.0);
        double newLength = 2.0;

        vector.setLength(newLength);  // ベクトルの長さを設定
        // vector = { 1.2, 1.6 }  (大きさが2.0に設定されたベクトル)
        ```

    -   原点を中心にベクトルを指定角度回転させる

        `rotated` メンバ関数を使用します。

        回転行列を用いて、ベクトルを指定した角度（ラジアン）だけ、回転させた新しいベクトルを返します。

        回転の中心は原点、回転方向は時計回りです。

        ```cpp
        Vec2 vector { 1.0, 0.0 };
        double angle = 45.0 * M_PI / 180;  // 45度をラジアンに変換

        Vec2 result = vector.rotated(angle);
        // result = { 0.707, 0.707 }  (45度回転したベクトル)
        ```

    -   指定した点を中心にベクトルを指定角度回転させる

        `rotatedAt` メンバ関数を使用します。

        回転行列を用いて、指定した回転中心（中心点）を基準にベクトルを指定した角度（ラジアン）だけ回転させた新しいベクトルを返します。

        回転方向は時計回りです。

        ```cpp
        Vec2 vector { 2.0, 1.0 };
        Vec2 center { 1.0, 1.0 };
        double angle = 90.0 * M_PI / 180.0;  // 90度をラジアンに変換

        Vec2 result = vector.rotatedAt(center, angle);  // ベクトルの回転
        // result = { 1.0, 0.0 }  (中心を軸にして90度回転したベクトル)
        ```
