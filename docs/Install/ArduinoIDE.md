# Arduino IDE

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

## Raspberry Pi Pico 使用時

本ライブラリを Raspberry Pi Pico で使用する際、 [arduino-pico](https://github.com/earlephilhower/arduino-pico/) (RP2040 ボード) のインストールが必要です。

1. Arduino IDE を開き、「ファイル」->「設定」に移動します。

2. 表示されるダイアログの「追加のボードマネージャー URL」フィールドに次の URL を入力します。

   ```
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```

3. 「ツール」-> 「ボード」->「ボードマネージャ」に移動し、検索窓に `Raspberry Pi Pico` と入力し、`Raspberry Pi Pico/RP2040` をインストールします。 (Arduino 公式ではない方をインストールすること)
