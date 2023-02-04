# library

香川高専高松機械システム研究部ライブラリ

## ライブラリ管理

ライブラリ管理に git を使用します。インストールしていない場合は [公式ページ](https://git-scm.com/download/win) からダウンロードし、インストールしてください。

-   追加

    1. ライブラリパスを調べる

        ファイル > 環境設定 > スケッチブックの保存場所

        スケッチブックの保存場所欄にあるパスの階層に libraries ディレクトリがあります。ここがライブラリを追加するディレクトリになっています。

    2. 1 で調べたディレクトリに移りクローンする

        またライブラリはプライベートであるため、クローンするには udonrobo organization に 参加している github アカウントと、 git が紐づいている必要があります。

        ```sh
        # 1で調べたディレクトリに移動
        cd C:\Users\----\Documents\Arduino\libraries

        # ライブラリをクローン
        git clone https://github.com/udonrobo/Library.git
        ```

-   更新

    ライブラリ追加時に移動したディレクトリに移り、`git pull`を実行

    ```sh
    # 追加する時に調べたディレクトリに移動
    cd C:\Users\----\Documents\Arduino\libraries

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

