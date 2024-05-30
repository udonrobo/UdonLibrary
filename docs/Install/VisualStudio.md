# Visual Studio

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/e7fa4cda-bdec-4d2f-b520-6cedb13b3f20" height="100px" align="right">

## Arduino のライブラリと共用する (推奨)

PC 内に本ライブラリ複数を置きたくない場合、Arduino のライブラリディレクトリに本ライブラリをクローンし、そこをインクルードパスとすることで、PC 内に本ライブラリを一つにできます。クローン方法 [Install: Arduino IDE](ArduinoIDE.md)

1. クローン [Arduino IDE 追加](ArduinoIDE.md)
2. [インクルードパス設定](#インクルードパス設定)

## Arduino のライブラリと共用しない

```sh
# VisualStudioのソリューションディレクトリで実行
git clone --recursive https://github.com/udonrobo/UdonLibrary.git
```

次のようなディレクトリ構成になっていれば OK です。

```sh
Test   <-- ソリューションディレクトリ
│  Test.sln
│
├─UdonLibrary   <--- うどん
│   ├─src
│   │  │  Udon.hpp
│   │  │
│   │  └─Udon
│   │      ├─
│
└─Test   <-- プロジェクトディレクトリ
    │  Test.cpp
    │  Test.vcxproj
    │  Test.vcxproj.filters
    │  Test.vcxproj.user
    │
        ...    ...
```

## インクルードパス設定

インクルードパスを設定することで `#include <Udon.hpp>` のように `#include <>` を使用して記述できるようになります。

インクルードパスの設定画面を次の手順で開きます。

1. プロジェクトのプロパティ編集ダイアログを開く

   ソリューションエクスプローラ > プロジェクトを右クリック > プロパティ

   ![image](https://github.com/udonrobo/UdonLibrary/assets/91818705/5eaa873d-64ac-478d-b999-d5a872aa2cd3)

2. インクルードディレクトリ編集ダイアログを開く

   VC++ディレクトリ > インクルードディレクトリの項目にある `↓` > 編集

   ![image](https://github.com/udonrobo/UdonLibrary/assets/91818705/5753824d-75e5-454d-947f-051f8fe87017)

   ![image](https://github.com/udonrobo/UdonLibrary/assets/91818705/99f3a95e-a2c3-4394-ba20-accc13126711)

3. インクルードディレクトリを追加する

   新しい行の追加(フォルダアイコンボタン)

   新しい項目に `$(SolutionDir)\UdonLibrary\src\` を追加します。`$(SolutionDir)` は ソリューションディレクトリのパスを表すマクロです。

   ![image](https://github.com/udonrobo/UdonLibrary/assets/91818705/10180b70-d664-435e-b366-156226863c68)

   ![image](https://github.com/udonrobo/UdonLibrary/assets/91818705/5aff2346-83d3-45ef-a8cc-89e1224a28d5)

4. 追加確認

   追加が成功している場合、写真の様に Udon.hpp のインクルードができるようになっています。

   ![image](https://github.com/udonrobo/UdonLibrary/assets/91818705/bf559aad-c13f-4a28-a360-e886483e3dbd)

## 更新

```sh
# UdonLibrary内で実行
git pull
```
