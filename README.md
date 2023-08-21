# UdonLibrary

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/a2536564-4f7e-452f-aea4-c76ecd03164b" height="200px" align="right"/>

香川高専高松機械システム研究部 C++ ライブラリ

[![Arduino Lint](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml)
[![Unit Tests](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml)

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

  ArduinoIDE はライブラリを置く専用のディレクトリがあるので、そこへクローンします。

  既定値: `~/Documents/Arduino/libraries`

  見つからない場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `/libraries`

  ```sh
  cd ~/Documents/Arduino/libraries
  git clone --recursive https://github.com/udonrobo/UdonLibrary.git
  ./UdonLibrary/setup.sh
  ```

- 更新

  追加する際に調べた `libraries` ディレクトリに移動し、プルします。

  ```sh
  cd ~/Documents/Arduino/libraries/UdonLibrary
  git pull
  ```

</details>

<details>
<summary> Platform IO </summary>

- 追加

  プロジェクトの `lib` ディレクトリへクローンすることで追加できます。

  ```sh
  git clone --recursive https://github.com/udonrobo/UdonLibrary.git
  ./UdonLibrary/setup.sh
  ```

- 更新

  プロジェクト内の lib ディレクトリでプルします。

  ```sh
  git pull
  ```

</details>

<details>
<summary> Visual Studio </summary>

プロジェクトディレクトリまたはソリューションディレクトリにクローンし、インクルードパスを設定することで使用できます。

- 追加

  追加先が git で管理されている場合

  ```sh
  # VisualStudioのプロジェクトディレクトリで実行
  git submodule add https://github.com/udonrobo/UdonLibrary.git
  git commit -m "add UdonLibrary"
  ```

  追加先が git で管理されていない場合

  ```sh
  # VisualStudioのプロジェクトディレクトリで実行
  git clone --recursive https://github.com/udonrobo/UdonLibrary.git
  ```

  > 次のようなディレクトリ構成になっていれば OK です。(プロジェクトディレクトリへ追加した場合)
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

- インクルードパス設定

  インクルードパスを設定することで `#include <Udon.hpp>` のように記述できるようになります。

  ソリューションエクスプローラ > プロジェクトを右クリック > プロパティ > VC++ディレクトリ > インクルードディレクトリの項目にある `↓` > 編集 > 新しい行の追加(フォルダアイコンボタン)

  新しい項目に `$(ProjectDir)\UdonLibrary\src\` を追加します。`$(ProjectDir)` は プロジェクトディレクトリのパスを表すマクロです。ソリューションディレクトリへ追加する場合 `$(SolutionDir)` を使用します。

- 追加している様子 (submodule 使用時)

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

`Udon.hpp` をインクルードすることで、全ての機能を使用できるようになります。

```cpp
#include <Udon.hpp>

void setup() {}
void loop() {}
```

```cpp
#include <Udon.hpp>

int main() {}
```

必要なヘッダーだけ個別にインクルードしたい場合 `UdonFwd.hpp` をインクルードします。ArduinoIDE 以外で使用する場合 `UdonFwd.hpp` のインクルードは不要です。

```cpp
#include <UdonFwd.hpp>
#include <Udon/Types/Vector2D.hpp>  // Udon::Vec2

void setup() {}
void loop() {}
```

```cpp
#include <Udon/Types/Vector2D.hpp>  // Udon::Vec2

int main() {}
```

各機能の使い方の詳細は次のドキュメント、[example](./example/) にあるスケッチ例を参照してください。

## Documents

- 通信
  - [CAN 通信](./docs/Communication/CAN.md)
  - [I2C 通信](./docs/Communication/I2C.md)
  - [UART 通信](./docs/Communication/UART.md)
  - [IM920](./docs/Communication/IM920.md)
  - [コントローラ](./docs/Communication/Pad.md)
  - [シリアライザ](./docs/Communication/Serialization.md)
- ユーザー定義型
  - [色空間](./docs/Types/Color.md)
  - [二次元ベクトル](./docs/Types/Vector2D.md)
  - [三次元ベクトル](./docs/Types/Vector3D.md)
  - [オイラー角](./docs/Types/Eular.md)
  - [クオータニオン](./docs/Types/Quaternion.md)
  - [固定ビット長浮動小数点型](./docs/Types/Float.md)
- 開発者用
  - [ディレクトリ構造](./docs/Developer/DirectoryStructure.md)
  - [自動テスト](./docs/Developer/CI.md)

## Acknowledgments

本ライブラリは以下のオープンソースプロジェクトを使用しています。

| プロジェクト                                                        | ライセンス                 | 著作権                                                             |
| ------------------------------------------------------------------- | -------------------------- | ------------------------------------------------------------------ |
| [Arduino](https://github.com/arduino/Arduino)                       | GNU General Public License | Copyright (c) 1989 1991 Free Software Foundation Inc.              |
| [FlexCAN_T4](https://github.com/tonton81/FlexCAN_T4)                | MIT License                | Copyright (c) 2019 Antonio Brewer                                  |
| [arduino-mcp2515](https://github.com/autowp/arduino-mcp2515)        | MIT License                | Copyright (c) 2013 Seeed Technology Inc. Copyright (c) 2016 Dmitry |
| [Adafruit_BusIO](https://github.com/adafruit/Adafruit_BusIO)        | MIT License                | Copyright (c) 2017 Adafruit Industries                             |
| [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)      | Apache-2.0 license         |                                                                    |
| [Adafruit_BNO055](https://github.com/adafruit/Adafruit_BNO055)      | MIT License                | Copyright (c) 2018 Adafruit Industries                             |
| [USB_Host_Shield_2.0](https://github.com/felis/USB_Host_Shield_2.0) | GNU General Public License |                                                                    |
| [OpenSiv3D](https://github.com/Siv3D/OpenSiv3D)                     | MIT License                | Copyright (c) 2008-2023 Ryo Suzuki, 2016-2023 OpenSiv3D Project    |
| [rclcpp](https://github.com/ros2/rclcpp)                            | Apache-2.0 license         |                                                                    |
| [compile-sketches](https://github.com/arduino/compile-sketches)     | GPL-3.0 license            | Copyright (c) 2007 Free Software Foundation Inc.                   |
| [GoogleTest](https://github.com/google/googletest)                  | BSD-3-Clause license       | Copyright (c) 2008 Google Inc.                                     |

また本ライブラリの作成において、以下のオープンソースプロジェクトを参考にしました。

| プロジェクト                                    | ライセンス           | 著作権                                                          |
| ----------------------------------------------- | -------------------- | --------------------------------------------------------------- |
| [OpenSiv3D](https://github.com/Siv3D/OpenSiv3D) | MIT License          | Copyright (c) 2008-2023 Ryo Suzuki, 2016-2023 OpenSiv3D Project |
| [cereal](https://github.com/USCiLab/cereal)     | BSD-3-Clause license | Copyright (c) 2013-2022 Randolph Voorhies, Shane Grant          |
| [boost](https://github.com/boostorg/boost)      | BSL-1.0 license      |                                                                 |
| [Unity](https://unity.com/ja)                   | MIT License          | Copyright (c) 2019 Unity Technologies Copyright                 |
