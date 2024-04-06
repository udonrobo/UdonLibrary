# 開発手順

## セットアップ

### 開発環境

自由ですが、ここでは VSCode, VisualStudio, ArduinoIDE を使用します。インストールしていない場合インストールしておいてください。

> 通常の開発では VSCode で記述 -> ArduinoIDE でコンパイル, 書き込み を繰り返し行います。
>
> マイコンを使用しないアルゴリズム系の開発の場合、デバッグのしやすい VisualStudio が便利です。最終的に ArduinoIDE でコンパイルできるか確認します。

### VSCode でライブラリを開く

通常 VSCode で本ライブラリを編集します。以下の写真の手順で VSCode でフォルダを開きます。

![alt text](image-3.png)

開けるとこのようになります。

![alt text](image-2.png)

### フォーマッタを clang-format に

フォーマッタとはコードを自動整形してくれるプログラムです。中でも clang-format は詳細に整形の設定を行えるので、本ライブラリで採用しています。設定ファイルは `UdonLibrary/.clang-format` にあります。

#### VS Code

[拡張機能: Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) を入れることで clang-format が使用できるようになります。

#### Visual Studio

ツールタブからオプション選択しダイアログを開きます。以下の写真に示すように ClangFormat サポートの有効化にチェックを入れるだけ！

![alt text](image-1.png)
