# Visual Studio

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/e7fa4cda-bdec-4d2f-b520-6cedb13b3f20" height="100px" align="right">

## Arduino のライブラリと共用する (推奨)

PC 内に本ライブラリ複数を置きたくない場合、Arduino のライブラリディレクトリに本ライブラリをクローンし、そこをインクルードパスとすることで、PC 内に本ライブラリを一つにできます。クローン方法 [Install: Arduino IDE](ArduinoIDE.md)

1. クローン [Arduino IDE 追加](ArduinoIDE.md)
2. [インクルードパス設定](#インクルードパス設定)

## ソリューションディレクトリに追加

1. ターミナルをソリューションディレクトリ下で起動

   ソリューションエクスプローラ > プロジェクトを右クリック > ターミナルを開く

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/2c7f4cd2-797d-4ea7-89a8-c898e07fd7b5">

2. ライブラリをクローン

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/a23bb96c-0ce2-4b64-af96-2a96abe1dd0f">

   ```sh
   git clone --recursive https://github.com/udonrobo/UdonLibrary.git
   ```

3. プロパティダイアログを開く

   ソリューションエクスプローラ > プロジェクトを右クリック > プロパティ

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/38d4991b-33c7-47e9-abfa-da1d93cb878d">

4. インクルードディレクトリ編集ダイアログを開く

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/0f4ce72a-0ee7-4ace-9e82-22bfd211ebf3">

5. インクルードディレクトリを追加する

   新しい項目に `$(SolutionDir)\UdonLibrary\src\` を追加します。`$(SolutionDir)` は ソリューションディレクトリのパスを表すマクロです。

   インクルードパスを設定することで `#include <Udon.hpp>` のように記述できるようになります。

   <img width=500px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/eb679ec6-cff9-447b-90bd-fc15bd0d031a">

6. 追加確認

   追加が成功している場合、写真の様に Udon.hpp のインクルードができるようになっています。

   <img width=400px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/bf559aad-c13f-4a28-a360-e886483e3dbd">
   
## 更新

1. `追加 1.` と同じくターミナルを起動します。

2. ライブラリディレクトリに移動しプルします。

   ```sh
   cd UdonLibrary
   git pull
   ```

   <img width=500px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/6524fd64-c87e-4763-8281-a643feab1aa1">
