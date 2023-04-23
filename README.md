# UdonLibrary

香川高専高松機械システム研究部 Arduino ライブラリ

[![Sketch Build](https://github.com/udonrobo/UdonLibrary/actions/workflows/arduino-cli.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/arduino-cli.yml)

## ライブラリ管理

-   git

    ライブラリ管理に git を使用します。インストールしていない場合は [公式ページ](https://git-scm.com/download/win) からダウンロードし、インストールしてください。

    > このライブラリのレポジトリはプライベートです。クローンするには udonrobo organization に 参加している github アカウントと、 git が紐付いている必要があります。git インストール後、初回のクローン時に紐付けを求められます。

-   追加

    ライブラリを保存するディレクトリを調べライブラリを追加します。

    > 既定値: `~\Documents\Arduino\libraries`
    >
    > 変更している場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `\libraries`

    > ```sh
    > # 調べたディレクトリに移動
    > cd ~\Documents\Arduino\libraries
    >
    > # ライブラリをクローン
    > git clone https://github.com/udonrobo/UdonLibrary.git
    > ```

-   更新

    > ```sh
    > # 追加する時に調べたlibrariesディレクトリに移動
    > cd ~\Documents\Arduino\libraries\UdonLibrary
    >
    > # ライブラリを更新
    > git pull
    > ```

## 機能一覧

-   アクチュエーター

    -   モーター

        > `udon::Motor2`
        >
        > `udon::Motor3`

    -   サーボ

        > --

-   アルゴリズム

    -   フィードバック制御器

        > `udon::PidController`

    -   移動平均

        > `udon::MovingAverage`

-   通信

    -   CAN 通信

        > `udon::CanBusTeensy`
        >
        > `udon::CanBusSpi`
        >
        > `udon::CanReader`
        >
        > `udon::CanWriter`

    -   I2C 通信

        > --

    -   SPI 通信

        > --

    -   UART 通信

        > --

    -   シリアライゼーション

        > `udon::Serializer`

    -   コントローラー

        > `udon::PadPS5`
        >
        > `udon::PadPS4`

-   ファイルシステム

    -   SD カードロガー

        > `udon::Parameter`

-   センサー

    -   BNO055

        > --

-   システム

    -   ループ周期制御

        > `udon::LoopCycleController`

-   ユーザー定義型

    -   固定 bit 長浮動小数点型

        > `udon::float16_t`, `udon::float32_t`, `udon::float64_t`, `udon::float80_t`, `udon::float128_t`

    -   ベクトル演算

        > `udon::Vector2D`: 二次元ベクトル
        >
        > `udon::Vector3D`: 三次元ベクトル
        >
        > `udon::Vector4D`: 四次元ベクトル
        >
        > `udon::Position`: ロボットの位置
        >
        > `udon::Quaternion`: クオータニオン

    -   線

        > `udon::Spline2D`: 二次元スプライン曲線
        >
        > `udon::Spline3D`: 三次元スプライン曲線
