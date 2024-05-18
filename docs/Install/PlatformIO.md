# Platform IO

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/b00cf135-38be-4b9b-97e6-0d5de337d1a3" height="100px" align="right">

## Arduino のライブラリと共用する (推奨)

PC 内に本ライブラリ複数を置きたくない場合、Arduino のライブラリディレクトリに本ライブラリをクローンし、そこをインクルードパスとすることで、本ライブラリを一つにすることができます。クローン方法 [Install: Arduino IDE](ArduinoIDE.md)

`platformio.ini` ファイルにライブラリのパスを設定します。

```ini
lib_deps =
    ~/Documents/Arduino/libraries/UdonLibrary
```

## Arduino のライブラリと共用しない

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
