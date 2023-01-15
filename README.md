# library

香川高専高松ライブラリ

## Adruino ライブラリに追加

1. ライブラリパスを調べる

    ArduinoIDE を起動 > ファイル > 環境設定 > スケッチブックの保存場所

    スケッチブックの保存場所欄にあるパスの階層に libraries ディレクトリがあります

2. 1 で調べたディレクトリに移りクローンする

    ```sh
    cd ---\arduino\programs\libraries  ## 1で調べたディレクトリ
    git clone https://github.com/udonrobo/Library.git
    ```

## ディレクトリ構造

-   スケッチ例、開発用ディレクトリ

    -   actuator

        アクチュエータ関連

    -   algorithm

        アルゴリズム関連

    -   com

        通信関連

        [CAN通信](./com/can/README.md)

        [I2C](./com/i2c/README.md)

    -   sensor

        センサー関連

    -   stl

        C++標準ライブラリ関連

    -   assembly.ps1

        Arduino のライブラリとして使用できるよう src ディレクトリにヘッダーを集めてくるスクリプト

-   ライブラリディレクトリ

    -   src

        ArduinoIDE からインクルードされるヘッダー群

## 開発の流れ

1. 開発用ディレクトリにてヘッダーファイル等を作成

2. assembly.ps1 の sources 配列に開発したファイルのパス(library ディレクトリからの相対パス)を追加

3. assembly.ps1 を powershell で実行(src ディレクトリに開発したファイルがコピーされます)
