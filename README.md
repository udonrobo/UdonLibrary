# UdonLibrary

香川高専高松機械システム研究部 Arduino ライブラリ

[![Sketch Build](https://github.com/udonrobo/UdonLibrary/actions/workflows/arduino-cli.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/arduino-cli.yml)

## ダウンロード

<details>
<summary> git </summary>

ライブラリのバージョン管理に git を使用します。インストールしていない場合はインストールしてください。

-   公式ページから

    https://git-scm.com/download

-   ターミナルから

    winget (windows)

    ```sh
    winget install --id Git.Git -e --source winget
    ```

    apt (linux)

    ```sh
    sudo apt-get update
    sudo apt-get install git-all
    ```

    homebrew (mac os 等)

    ```sh
    brew install git
    ```

    インストール確認

    ```sh
    git version
    ```

> 本ライブラリのレポジトリはプライベートです。クローンするには udonrobo organization に 参加している github アカウントと、 git が紐付いている必要があります。git インストール後、初回のクローン時に紐付けを求められます。

</details>

<details>
<summary> ArduinoIDE </summary>

-   追加

    Arduinoにはライブラリを保存する特定のディレクトリがあります。その場所を見つけて、新しいライブラリを追加します。

    > 既定値: `~/Documents/Arduino/libraries`
    >
    > 変更している場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `/libraries`

    > ```sh
    > # ターミナルを開き、調べたディレクトリに移動
    > cd ~/Documents/Arduino/libraries
    >
    > # 本ライブラリクローン
    > git clone https://github.com/udonrobo/UdonLibrary.git
    >
    > # セットアップ実行(依存ライブラリの追加等)
    > ./UdonLibrary/setup.sh
    > ```

-   更新

    > ```sh
    > # 追加する時に調べたlibrariesディレクトリに移動
    > cd ~/Documents/Arduino/libraries/UdonLibrary
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
        git submodule add -b master https://github.com/udonrobo/UdonLibrary.git ./プロジェクトディレクトリ/UdonLibrary
        ```

    -   プロジェクトがリポジトリでない場合

        ```sh
        cd ソリューションディレクトリ/プロジェクトディレクトリ
        git clone https://github.com/udonrobo/UdonLibrary.git
        ```

2.  インクルードパス追加


    以下の手順で操作します。

    ソリューションエクスプローラ > プロジェクトを右クリック > プロパティ > VC++ディレクトリ > インクルードディレクトリの項目にある `↓` > 編集 > 新しい行の追加(フォルダアイコンボタン)

    新しい項目に `$(ProjectDir)\UdonLibrary\src\` と入力します。`$(ProjectDir)` は プロジェクトディレクトリのパスを表すマクロです。

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

`UdonFwd.hpp` をインクルードすることで、必要なヘッダーだけ個別にインクルードできるようになり、コンパイル時間を短縮できます。

```cpp
#include <UdonFwd.hpp>
#include <Udon/Types/Vector2D.hpp>  // Udon::Vec2

void setup() {}
void loop() {}
```

各機能の使い方の詳細は [docs](./docs/) にあるドキュメント、 [example](./example/) にあるスケッチ例を参照してください。
