# Arduino IDE

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/e5ae01ac-75d3-40ca-a8ba-42af4cc8af9d" height="100px" align="right">

以下の手順の追加を行います

1. Arduino ライブラリディレクトリへ移動
2. ライブラリのクローン

## 追加

1. ターミナルを起動

   Windows の場合、Windows ボタンを押し、ターミナルと検索すると起動できます。

   > ターミナルがない場合 Microsoft Store でインストールできます。
   >
   > <https://www.microsoft.com/store/productId/9N0DX20HK701?ocid=pdpshare>

2. クローン

   以下コマンドをターミナル上で実行します。

   ```sh
   cd ~/Documents/Arduino/libraries
   git clone --recursive https://github.com/udonrobo/UdonLibrary.git
   ```

   > ライブラリディレクトリ既定値: `~/Documents/Arduino/libraries`
   >
   > 見つからない場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `/libraries`

   本ライブラリは C++ 標準ライブラリに依存しています。標準ライブラリが使用できない環境 (Arduino Nano 等) で使用するには UdonArduinoSTL を合わせて追加してください。

   ```sh
   git clone https://github.com/udonrobo/UdonArduinoSTL.git
   ```

3. 追加確認

   追加が成功している場合、以下の様に Udon.hpp のインクルードができるようになっています。

   <img width=400px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/258f3ff9-570f-4448-b9c8-5665c2d195ef">

## 更新

ライブラリのディレクトリでプルします。

```
cd ~/Documents/Arduino/libraries/UdonLibrary
git pull
```
