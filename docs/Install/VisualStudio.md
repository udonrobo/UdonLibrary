# Visual Studio <img src="https://github.com/CaseyNelson314/Visualizer/assets/91818705/d190da8e-dc18-46fc-90a1-f4258b1df4b1" height="100px" align="right">

## Arduino のライブラリと共用する場合

PC 内に本ライブラリ複数を置きたくない場合、Arduino のライブラリディレクトリに本ライブラリをクローンし、そこをインクルードパスとすることで、本ライブラリを一つにすることができます。クローン方法 [Install: Arduino IDE](ArduinoIDE.md)

## 追加

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

## インクルードパス設定

インクルードパスを設定することで `#include <Udon.hpp>` のように `#include <>` を使用して記述できるようになります。

インクルードパスの設定画面を次の手順で開きます。

ソリューションエクスプローラ > プロジェクトを右クリック > プロパティ > VC++ディレクトリ > インクルードディレクトリの項目にある `↓` > 編集 > 新しい行の追加(フォルダアイコンボタン)

### Arduino ライブラリディレクトリ使用時

新しい項目に `$(HOMEPATH)\Documents\Arduino\UdonLibrary\src\` を追加します。

`$(HOMEPATH)` はホームディレクトリ `~/` を表すマクロです。

### 通常クローン、submodule 使用時

新しい項目に `$(ProjectDir)\UdonLibrary\src\` を追加します。`$(ProjectDir)` は プロジェクトディレクトリのパスを表すマクロです。

追加している様子 (submodule 使用時)

![setup](https://github.com/udonrobo/UdonLibrary/assets/91818705/aaecedbc-2490-4b11-85e5-fbf0a7d09302)

[サンプルレポジトリ](https://github.com/udonrobo/UdonLibraryDemoOnVisualStudio)

## 更新

### 通常クローン、Arduino ライブラリディレクトリ使用時

```sh
# UdonLibrary内で実行
git pull
```

### submodule 使用時

```sh
# 追加先のリポジトリ内で実行
git submodule update --remote
```

## submodule 使用時の注意点

> 追加先のプロジェクト自体のクローンを行うとき `--recursive` オプションを与えなければ、ライブラリがクローンされません(空ファイルになります)。
>
> ```sh
> git clone --recursive <プロジェクトURL>
> ```

> また submodule は追加時のコミットを参照するため、追加先のプロジェクトをクローンしても、ライブラリは submodule 追加時のコミットの内容になります。
>
> 最新のライブラリを使用する場合は submodule を更新する必要があります。
>
> ```sh
> git submodule update --remote
> ```
