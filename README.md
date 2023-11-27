# UdonLibrary [![Arduino Lint](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml) [![Unit Tests](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml)

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/53d063f8-ee25-41c9-b68b-75dcfd8ee5ad" height="300px" align="right"/>

香川高専高松機械システム研究部 C++ ライブラリ

## Install

<details>
<summary> Git </summary>

ライブラリのバージョン管理に git を使用します。インストールしていない場合はインストールしてください。

- 公式ページから

  <https://git-scm.com/download>

- Windows

  ```sh
  winget install --id Git.Git -e --source winget
  ```

- Linux

  ```sh
  sudo apt-get update && sudo apt-get install git-all
  ```

- macOS or Linux

  ```sh
  brew install git
  ```

> 本ライブラリのレポジトリはプライベートであるため、クローンするには udonrobo organization に 参加している github アカウントと、 git が紐付いている必要があります。git インストール後、初回のクローン時に紐付けを求められます。

</details>

<details>
<summary> ArduinoIDE </summary>

### 追加

ArduinoIDE はライブラリを置く専用のディレクトリがあるので、そこへクローンします。

既定値: `~/Documents/Arduino/libraries`

見つからない場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `/libraries`

```sh
cd ~/Documents/Arduino/libraries
git clone --recursive https://github.com/udonrobo/UdonLibrary.git
./UdonLibrary/setup.sh
```

### 更新

追加した `UdonLibrary` ディレクトリでプルします。

```sh
cd ~/Documents/Arduino/libraries/UdonLibrary
git pull
```

</details>

<details>
<summary> Platform IO </summary>

### Arduino のライブラリと共用する場合

PC 内に本ライブラリ複数を置きたくない(更新を個別で行うのが面倒等)場合、Arduino のライブラリディレクトリに本ライブラリをクローンし、インクルードパスを設定することで、本ライブラリを一つにすることができます。ArduinoIDE の追加を参考にライブラリをクローンしてください。

`platformio.ini` ファイルにライブラリのパスを設定します。

```ini
lib_deps =
    ~/Documents/Arduino/libraries/UdonLibrary
```

### 追加

プロジェクトの `lib` ディレクトリへクローンすることで追加できます。

```sh
git clone --recursive https://github.com/udonrobo/UdonLibrary.git
./UdonLibrary/setup.sh
```

### 更新

追加した `UdonLibrary` ディレクトリでプルします。

```sh
git pull
```

</details>

<details>
<summary> Visual Studio </summary>

### Arduino のライブラリと共用する場合

PC 内に本ライブラリ複数を置きたくない(更新を個別で行うのが面倒等)場合、Arduino のライブラリディレクトリに本ライブラリをクローンし、インクルードパスを設定することで、本ライブラリを一つにすることができます。ArduinoIDE の追加を参考にライブラリをクローンしてください。

### 追加

追加先が git で管理されている場合、リポジトリ内にリポジトリを置くことになります。このような場合、submodule の使用が推奨されています。

```sh
# VisualStudioのプロジェクトディレクトリで実行
git submodule add https://github.com/udonrobo/UdonLibrary.git
git commit -m "add UdonLibrary"
```

追加先が git で管理されていない場合、通常クローンを行います。

```sh
# VisualStudioのプロジェクトディレクトリで実行
git clone --recursive https://github.com/udonrobo/UdonLibrary.git
```

次のようなディレクトリ構成になっていれば OK です。

```sh
Test   <-- ソリューションディレクトリ
│  Test.sln
│
└─Test   <-- プロジェクトディレクトリ
    │  Test.cpp
    │  Test.vcxproj
    │  Test.vcxproj.filters
    │  Test.vcxproj.user
    │
    └─UdonLibrary   <--- うどん
        ├─src
        │  │  Udon.hpp
        │  │
        │  └─Udon
        │      ├─
        ...    ...
```

### インクルードパス設定

インクルードパスを設定することで `#include <Udon.hpp>` のように `<>` を使用して記述できるようになります。

C++インクルードパスを設定画面を次の手順で開きます。

> ソリューションエクスプローラ > プロジェクトを右クリック > プロパティ > VC++ディレクトリ > インクルードディレクトリの項目にある `↓` > 編集 > 新しい行の追加(フォルダアイコンボタン)

- clone submodule を用いて追加した場合

  新しい項目に `$(ProjectDir)\UdonLibrary\src\` を追加します。`$(ProjectDir)` は プロジェクトディレクトリのパスを表すマクロです。

  追加している様子 (submodule 使用時)

  ![setup](https://github.com/udonrobo/UdonLibrary/assets/91818705/aaecedbc-2490-4b11-85e5-fbf0a7d09302)

  [サンプルレポジトリ](https://github.com/udonrobo/UdonLibraryDemoOnVisualStudio)

- Arduino ライブラリディレクトリに追加した場合

  新しい項目に `$(HOMEPATH)\Documents\Arduino\UdonLibrary\src\` を追加します。

  `$(HOMEPATH)` はホームディレクトリ `~/` を表すマクロです。

### 更新

通常

```sh
# UdonLibrary内で実行
git pull
```

submodule 使用時

```sh
# 追加先のリポジトリ内で実行
git submodule update --remote
```

### `git submodule` 使用時の注意点

> 追加先のプロジェクト自体のクローンを行うとき `--recursive` オプションを与えなければライブラリがクローンされません(空フォルダになります)。
>
> ```sh
> git clone --recursive <プロジェクトURL>
> ```

> また submodule は追加時のコミットを参照するため、追加先のプロジェクトをクローンしても、ライブラリは submodule 追加時のコミットの内容になります。
>
> 最新のライブラリを使用する場合は submodule を更新する必要があります。(submodule 使用時の更新を参照)

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

各機能の使い方の詳細は以下のドキュメント、[example](./example/) にあるスケッチ例を参照してください。

## Documents

### アルゴリズム

- [独立ステア最適化](./docs/Algorithm/SteerOptimizer.md)

### 通信

- [共通](./docs/Communication/Common.md)
- [CAN](./docs/Communication/CAN.md)
- [I2C](./docs/Communication/I2C.md)
- [UART](./docs/Communication/UART.md)
- [IM920](./docs/Communication/IM920.md)
- [コントローラ](./docs/Communication/Pad.md)
- [シリアライザ](./docs/Communication/Serialization.md)

### ユーザー定義型

- [色空間](./docs/Types/Color.md)
- [二次元ベクトル](./docs/Types/Vector2D.md)
- [三次元ベクトル](./docs/Types/Vector3D.md)
- [オイラー角](./docs/Types/Eular.md)
- [クオータニオン](./docs/Types/Quaternion.md)
- [固定ビット長浮動小数点型](./docs/Types/Float.md)

### ドライバー

- [ロータリーエンコーダー](./docs/Driver/Encoder.md)

### 言語

- [型トレイト](./docs/Other/Traits.md)

### 開発者用

- [コンテナ](./docs/Algorithm/Container.md)
- [ディレクトリ構造](./docs/Developer/DirectoryStructure.md)
- [自動テスト](./docs/Developer/CI.md)
- [謝辞](./docs/Other/Acknowledgments.md)
