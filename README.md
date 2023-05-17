# UdonLibrary

香川高専高松機械システム研究部 Arduino ライブラリ

[![Sketch Build](https://github.com/udonrobo/UdonLibrary/actions/workflows/arduino-cli.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/arduino-cli.yml)

## ダウンロード

<details open>
<summary> Git </summary>

ライブラリ管理に Git を使用します。インストールしていない場合は [公式ページ](https://git-scm.com/download/win) からダウンロードし、インストールしてください。

> このライブラリのレポジトリはプライベートです。クローンするには udonrobo organization に 参加している github アカウントと、 git が紐付いている必要があります。git インストール後、初回のクローン時に紐付けを求められます。

</details>

<details open>
<summary> ArduinoIDE </summary>

-   追加

    ライブラリを保存するディレクトリを調べ、ライブラリを追加します。

    > 既定値: `~\Documents\Arduino\libraries`
    >
    > 変更している場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `\libraries`

    > ```sh
    > # ターミナルを開き、調べたディレクトリに移動
    > cd ~\Documents\Arduino\libraries
    >
    > # 必要なライブラリをクローン
    > git clone https://github.com/udonrobo/UdonLibrary.git
    > git clone https://github.com/udonrobo/UdonArduinoSTL.git
    > git clone https://github.com/autowp/arduino-mcp2515.git
    > git clone https://github.com/adafruit/Adafruit_BusIO.git
    > git clone https://github.com/adafruit/Adafruit_Sensor.git
    > git clone https://github.com/adafruit/Adafruit_BNO055.git
    > ```

-   更新

    > ```sh
    > # 追加する時に調べたlibrariesディレクトリに移動
    > cd ~\Documents\Arduino\libraries\UdonLibrary
    >
    > # ライブラリを更新
    > git pull
    > ```

</details>

<details>
<summary> Visual Studio </summary>

1.  クローン

    -   プロジェクトがリポジトリにある場合

        git submodule を使用して追加します。

        ```sh
        cd ソリューションディレクトリ
        git submodule add -b master https://github.com/udonrobo/UdonLibrary.git .\プロジェクトディレクトリ\UdonLibrary
        ```

    -   プロジェクトがリポジトリでない場合

        ```sh
        cd ソリューションディレクトリ\プロジェクトディレクトリ
        git clone https://github.com/udonrobo/UdonLibrary.git
        ```

2.  インクルードパス追加

    ソリューションエクスプローラ > プロジェクト > プロパティ > VC++ディレクトリ > インクルードディレクトリの項目にある `↓` > 編集 > 新しい行の追加(フォルダアイコンボタン)

    に `$(ProjectDir)\UdonLibrary\src\` と入力します。`$(ProjectDir)` は プロジェクトディレクトリのパスを表すマクロです。

3.  ディレクトリツリー確認

    次のようなディレクトリ構成になっていれば OK です。

    ```
    test   <-- ソリューションディレクトリ
    │  .gitmodules  <--- git submodule 使用時に生成
    │  test.sln
    │
    └─test   <-- プロジェクトディレクトリ
        │  test.cpp
        │  test.vcxproj
        │  test.vcxproj.filters
        │  test.vcxproj.user
        │
        └─UdonLibrary   <--- うどん
            ├─src
            │  │  Udon.hpp
            │  │
            │  └─udon
            │      ├─
            ...    ...
    ```

-   追加してる図

    ![setup](https://github.com/udonrobo/UdonLibrary/assets/91818705/ab1b9ac1-58a4-4423-aa06-64db38957d08)

</details>

## 使用方法

`Udon.hpp` をインクルードすることで、全機能を使用できるようになります。

```cpp
#include <Udon.hpp>

void setup() {}
void loop() {}
```

`UDON_INCLUDE_SELECTABLE` を定義することで、個別にインクルードすることもできます。コンパイル時間を短縮することができます。

```cpp
#define UDON_INCLUDE_SELECTABLE

#include <Udon.hpp>
#include <udon/types/Vector2D.hpp>  // udon::Vec2

void setup() {}
void loop() {}
```

各機能の使い方の詳細は [docs](./docs/) にあるドキュメント、 [example](./example/) にあるスケッチ例を参照してください。

## 機能一覧

-   アクチュエーター

    -   モーター

        > `udon::Motor2`
        >
        > `udon::Motor3`

    -   サーボ

        > `udon::ServoSpeed`

-   アルゴリズム

    -   コンテナ

        > `udon::ArrayView`: 配列参照
        >
        > `udon::RingBuffer`: 静的可変長配列

    -   フィードバック制御器

        > `udon::PidController`

    -   移動平均

        > `udon::MovingAverage`

    -   ループ周期制御

        > `udon::LoopCycleController`

-   通信

    -   CAN 通信

        > `udon::Can20ABus`
        >
        > `udon::CanReader`
        >
        > `udon::CanWriter`

    -   I2C 通信

        > `udon::I2cBus`
        >
        > `udon::I2cMasterReader`
        >
        > `udon::I2cMasterWriter`
        >
        > `udon::I2cSlaveReader`
        >
        > `udon::I2cSlaveWriter`

    -   SPI 通信

        > --

    -   UART 通信

        > `udon::UartBus`
        >
        > `udon::UartReader`
        >
        > `udon::UartWriter`

    -   シリアライゼーション

        > `udon::Pack`, `udon::Unpack`

    -   コントローラー

        > `udon::PadPS5`
        >
        > `udon::PadPS4`

-   ファイルシステム

    -   SD カードロガー

        > `udon::Parameter`

-   センサー

    -   慣性計測装置

        > `udon::Imu`
        >
        > `udon::BNO055`

-   ユーザー定義型

    -   固定ビット長浮動小数点型

        > `udon::float16_t`, `udon::float32_t`, `udon::float64_t`, `udon::float80_t`, `udon::float128_t`

    -   座標系

        > `udon::Vector2D`: 二次元ベクトル
        >
        > `udon::Vector3D`: 三次元ベクトル
        >
        > `udon::Vector4D`: 四次元ベクトル
        >
        > `udon::Position`: ロボットの位置
        >
        > `udon::Polar`: 極座標
        >
        > `udon::Quaternion`: クオータニオン

    -   線

        > `udon::Spline2D`: 二次元スプライン曲線
        >
        > `udon::Spline3D`: 三次元スプライン曲線
