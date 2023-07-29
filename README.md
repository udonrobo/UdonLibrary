# UdonLibrary

香川高専高松機械システム研究部 Arduino ライブラリ

[![Sketch Build](https://github.com/udonrobo/UdonLibrary/actions/workflows/arduino-cli.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/arduino-cli.yml)

## Install

<details>
<summary> git </summary>

ライブラリのバージョン管理に git を使用します。インストールしていない場合はインストールしてください。

- 公式ページから

  <https://git-scm.com/download>

- ターミナルから

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

> 本ライブラリのレポジトリはプライベートであるため、クローンするには udonrobo organization に 参加している github アカウントと、 git が紐付いている必要があります。git インストール後、初回のクローン時に紐付けを求められます。

</details>

<details>
<summary> ArduinoIDE </summary>

- 追加

  Arduino にはライブラリを保存する特定のディレクトリがあります。その場所を見つけて、新しいライブラリを追加します。

  > 既定値: `~/Documents/Arduino/libraries`
  >
  > 変更している場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `/libraries`
  >
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

- 更新

  ```sh
  # 追加する時に調べたlibrariesディレクトリに移動
  cd ~/Documents/Arduino/libraries/UdonLibrary

  # ライブラリを更新
  git pull
  ```

</details>

<details>
<summary> Visual Studio </summary>

1. 追加

   追加先が git で管理されている場合 -> submodule を使用

   ```sh
   # VisualStudioのプロジェクトディレクトリで実行
   git submodule add https://github.com/udonrobo/UdonLibrary.git

   git commit -m "add UdonLibrary" # submodule add でステージングも行われるため git add 不要
   ```

   追加先が git で管理されていない場合 -> クローン

   ```sh
   # VisualStudioのプロジェクトディレクトリで実行
   git clone https://github.com/udonrobo/UdonLibrary.git
   ```

   > 次のようなディレクトリ構成になっていれば OK です。
   >
   > ```sh
   > Test   <-- ソリューションディレクトリ
   > │  Test.sln
   > │
   > └─Test   <-- プロジェクトディレクトリ
   >     │  Test.cpp
   >     │  Test.vcxproj
   >     │  Test.vcxproj.filters
   >     │  Test.vcxproj.user
   >     │
   >     └─UdonLibrary   <--- うどん
   >         ├─src
   >         │  │  Udon.hpp
   >         │  │
   >         │  └─Udon
   >         │      ├─
   >         ...    ...
   > ```

2. インクルードパス設定

   以下の手順でインクルードパスを設定することで　`#include <Udon.hpp>`　のように記述できるようになります。

   ソリューションエクスプローラ > プロジェクトを右クリック > プロパティ > VC++ディレクトリ > インクルードディレクトリの項目にある `↓` > 編集 > 新しい行の追加(フォルダアイコンボタン)

   新しい項目に `$(ProjectDir)\UdonLibrary\src\` と入力します。`$(ProjectDir)` は プロジェクトディレクトリのパスを表すマクロです。

- 追加している様子 (submodule使用時)

  ![setup](https://github.com/udonrobo/UdonLibrary/assets/91818705/aaecedbc-2490-4b11-85e5-fbf0a7d09302)

  [サンプルレポジトリ](https://github.com/udonrobo/UdonLibraryDemoOnVisualStudio)

- 更新

  submodule 使用時

  ```sh
  # 追加先のリポジトリ内で実行
  git submodule update --remote
  ```

  通常クローン時

  ```sh
  # UdonLibrary内で実行
  git pull
  ```

- `git submodule` 使用時の注意点

  > 追加先のプロジェクト自体のクローンを行うとき `--recursive` オプションを与えなければライブラリがクローンされません(空フォルダになります)。
  >
  > ```sh
  > git clone --recursive <プロジェクトURL>
  > ```

  > また submodule は追加時のコミットを参照するため、追加先のプロジェクトをクローンしてもライブラリは submodule 追加時のコミットの内容になります。
  >
  > 最新のライブラリを使用する場合は submodule を更新する必要があります。(`git submodule` 使用時の更新を参照)

</details>

## Usage

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
