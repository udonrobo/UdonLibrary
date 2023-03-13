# library

香川高専高松機械システム研究部ライブラリ

## ライブラリ管理

-   git

    ライブラリ管理に git を使用します。インストールしていない場合は [公式ページ](https://git-scm.com/download/win) からダウンロードし、インストールしてください。

    またこのライブラリのレポジトリはプライベートであるため、クローンするには udonrobo organization に 参加している github アカウントと、 git が紐づいている必要があります。

-   追加

    libraries ディレクトリにライブラリを追加します。

    1. ライブラリを保存するパスを調べる

        既定値: `~\Documents\Arduino\libraries`

        変更している場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `\libraries`

    2. 1 で調べたディレクトリに移りクローンする

        ```sh
        # 1で調べたディレクトリに移動
        cd ~\Documents\Arduino\libraries

        # ライブラリをクローン
        git clone https://github.com/udonrobo/Library.git
        ```

-   更新

    ```sh
    # 追加する時に調べたlibrariesディレクトリに移動
    cd ~\Documents\Arduino\libraries

    # ライブラリを更新
    git pull
    ```

## ディレクトリ構造

スケッチ例、開発用ディレクトリ

-   actuator

    アクチュエータ

-   algorithm

    アルゴリズム

-   com

    通信

    [CAN](./com/can/README.md)

    <!-- [I2C](./com/i2c/README.md) -->

    <!-- [SPI](./com/spi/README.md) どなたか作ってぇぇぇぇぇ -->

-   sensor

    センサー

-   stl

    C++標準ライブラリ

-   assembly.ps1

    ライブラリとして使用できるように ./src にヘッダーを収集するスクリプト

ライブラリディレクトリ

-   src

    ライブラリのヘッダー群

## 開発

<details>
<summary>ファイル操作</summary>

1. 開発用ディレクトリでヘッダーファイル等を作成、または編集

2. 新規追加時の場合、`assembly.ps1` 内 `sources` 配列に開発したファイルのパスを追加

    `Library\hoge\hogehoge.hpp` ファイルを収集対象に追加する場合、`"${selfpath}\hoge\hogehoge.hpp"` の様にパスを指定し、配列に追加します。

    `selfpath` には `assembly.ps1` の親ディレクトリのパスが代入されています。

3. assembly.ps1 を powershell で実行

    src ディレクトリに開発したファイルがコピーされます

    ```sh
    cd ---\Library
    .\assembly.ps1
    ```

</details>

<details>
<summary>命名規則</summary>

-   禁止事項

    -   アンダースコアから始まる識別子名は禁止

        言語規格や処理系で使うために予約されている可能性があるため。また将来的に予約語になる可能性があるため。

-   ファイル名

    アッパーキャメルケース

    拡張子主に `.ino`, `.hpp`, `.cpp`

    `SampleClass.hpp`

-   変数

    ローワーキャメルケース

    ```cpp
    int i = 0;
    auto timeMs = millis();
    ```

-   定数

    -   define マクロ

        コンスタントケース

        名前衝突を防ぐためなるべく長く、なるべく undef する。

        ```cpp
        #define SERIAL_DATA_LENGTH (128)
        ```

    -   const 定数

        ローワーキャメルケース

        ```cpp
        const int id = GetID();
        const size_t dataLength = reader.length();
        ```

    -   constexpr 定数

        アッパーキャメルケース or コンスタントケース

        ```cpp
        constexpr int Size = 256;
        constexpr int DataSize = 1024;
        constexpr uint8_t LED_PIN = 13;
        ```

-   関数名

    -   グローバル関数

        アッパーキャメルケース

        なるべく動詞

        ```cpp
        void Init() {}
        uint8_t GetSerialData() {}
        ```

    -   メンバ関数名

        ローワーキャメルケース

        なるべく動詞(size, length 等はよく使われるのでﾖｼｯ)

        ```cpp
        void begin();
        double getData();
        ```

    -   関数クロージャー

        ローワーキャメルケース

        なるべく短く

        ```cpp
        const auto f = []() {};
        const auto read = []() {};
        ```

-   クラス、構造体名

    アッパーキャメルケース or スネークケース(テンプレートメタプログラミング用)

    名詞

    ```cpp
    class SerialWriter {};
    class PIDController {};  // PIDのような単語は頭文字をとった略語であるため、Pid のようにしない

    template<typename T, typename U>
    struct is_same {};
    ```

-   template パラメーター

    アッパーキャメルケース

    なるべく短く

    ```cpp
    template<class Ty>
    template<size_t Size>
    ```

-   特殊ケース

    C++ STL や Boost 等にあるクラス、関数名を模倣する場合、それぞれのライブラリの命名規則に従う。

</details>

<details>
<summary>テストケース</summary>

-   リンクエラー

    リンクエラーはきもいです(唐突)複数の .cpp ファイルから include した際にリンクエラーにならないようにしてください。主にヘッダーファイルに関数や変数の実体がある場合にリンクエラーになります。一つの .cpp ファイルから include する際はエラーにならないので注意が必要です。`#pragma once` は多重インクルードは防ぐことができますが、リンクエラーは防ぐことができません。

    -   エラー文

        ```
        gcc: multiple definition of `識別名名` 😭
        msvc: LNK2005 "public: void __cdecl Hoge::f(void)" (?f@Hoge@@QEAAXXZ) は既に erro.obj で定義されています。🤪🤪🤪
        ```

    -   変数

        ```cpp
        // エラー(変数実体がヘッダーに記述されているため)
        int value;
        ```

    -   関数

        ```cpp
        void f() {}  // NG
        inline void f() {} // OK
        ```

    -   クラス

        ```cpp
        class Hoge {
            static int value;
            void f();
        };
        int Hoge::value;         // NG
        void Hoge::f() {};       // NG
        inline void Hoge::f() {} // OK

        template<class T>
        class Hoge {
            static int value;
        };
        template<class T> Hoge<T>::value; // OK
        ```

-   メモリリーク

</details>
