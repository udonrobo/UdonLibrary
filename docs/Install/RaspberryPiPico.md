# Raspberry Pi Pico セットアップ

本ライブラリを Raspberry Pi Pico 環境で使用する際、 [arduino-pico](https://github.com/earlephilhower/arduino-pico/) (RP2040 ボード) のインストールが必要です。

## インストール手順

1. Arduino IDE を開き、「ファイル」->「設定」に移動します。

2. 表示されるダイアログの `追加のボードマネージャー URL` フィールドに次の URL を入力します。

   ```txt
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```

3. 「ツール」-> 「ボード」->「ボードマネージャ」に移動し、検索窓に `Raspberry Pi Pico` と入力し、`Raspberry Pi Pico/RP2040` をインストールします。 (Arduino 公式ではない方をインストールすること)
