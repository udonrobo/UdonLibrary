
## 開発

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
    class PidController {};

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

    複数の .cpp ファイルから include した際にリンクエラーにならないようにしてください。主にヘッダーファイルに関数や変数の実体がある場合にリンクエラーになります。一つの .cpp ファイルから include する際はエラーにならないので注意が必要です。`#pragma once` は多重インクルードは防ぐことができますが、リンクエラーは防ぐことができません。

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
