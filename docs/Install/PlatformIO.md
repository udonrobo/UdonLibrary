# Platform IO <img src="https://github.com/CaseyNelson314/Visualizer/assets/91818705/94ac076d-c61c-4ce2-92ea-a52a3227efd3" height="100px" align="right">

## Arduino のライブラリと共用する場合

PC 内に本ライブラリ複数を置きたくない場合、Arduino のライブラリディレクトリに本ライブラリをクローンし、そこをインクルードパスとすることで、本ライブラリを一つにすることができます。クローン方法 [Install: Arduino IDE](ArduinoIDE.md)

`platformio.ini` ファイルにライブラリのパスを設定します。

```ini
lib_deps =
    ~/Documents/Arduino/libraries/UdonLibrary
```

## 追加

プロジェクトの `lib` ディレクトリへクローンすることで追加できます。

```sh
git clone --recursive https://github.com/udonrobo/UdonLibrary.git
./UdonLibrary/setup.sh
```

## 更新

追加した `UdonLibrary` ディレクトリでプルします。

```sh
git pull
```
