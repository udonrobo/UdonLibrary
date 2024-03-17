# Arduino IDE <img src="https://github.com/CaseyNelson314/Visualizer/assets/91818705/dff786fa-ef51-4428-8cf6-6615ddd75eb4" height="100px" align="right">

## 追加

Arduino IDE にはライブラリを置く専用のディレクトリがあるので、そこへクローンします。

既定値: `~/Documents/Arduino/libraries`

見つからない場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `/libraries`

```sh
cd ~/Documents/Arduino/libraries
git clone --recursive https://github.com/udonrobo/UdonLibrary.git
./UdonLibrary/setup.sh
```

## 更新

追加した本ライブラリディレクトリでプルします。

```sh
cd ~/Documents/Arduino/libraries/UdonLibrary
git pull
```
