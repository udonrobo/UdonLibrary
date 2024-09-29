# 開発環境セットアップ

ライブラリの開発に直接関与しませんが、スムーズに開発できる環境を整える手順を記載します。

- [VSCode](#vscode)
- [Visual Studio](#visual-studio)
- [GitHub Copilot 導入](#github-copilot-導入)

## VSCode

ライブラリは VSCode で記述 -> ArduinoIDE でコンパイル, 書き込み の流れで開発します。VSCode は拡張機能をインストールすることで、コードの自動整形、スペルチェック、インテリセンスの機能を強化できます。

### ライブラリを開く

通常 VSCode で本ライブラリを編集します。以下の写真の手順で VSCode でフォルダを開きます。

<img width="600px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/1b12fd5c-d441-49a5-a60f-40f8ec238f6e"/>

> [!TIP]
>
> `code` コマンドで任意のディレクトリ、ファイルを開けます。以下のコマンドで本ライブラリを開くことができます。
>
> ```sh
> code ~/Arduino/libraries/UdonLibrary/
> ```

開けるとこのようになります。左に表示されるエクスプローラーからファイルを開けるので、そこから編集していきます。

<img width="600px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/38d7b8cb-eee8-491d-b913-037d7e07f507"/>

### clang-format 拡張機能インストール

clang-format とはフォーマッタの一種で、コードを自動整形してくれるツールです。整形の設定を詳細に行えるので本ライブラリで採用しています。設定ファイルは `UdonLibrary/.clang-format` にあります。コンパイラ基盤 LLVM に付属しています。

1. 以下コマンドで LLVM をインストールします。

   ```sh
   winget install LLVM
   ```

2. 拡張機能をインストールします。[Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)

3. 拡張機能の設定を開き、実行ファイルのパスを指定します。`C:\Program Files\LLVM\bin\clang-format.exe`

   <img width="600px" src="https://github.com/user-attachments/assets/fb53705f-ad82-4c46-a34c-e87c651b87a8"/>

   <img width="600px" src="https://github.com/user-attachments/assets/2d635fb4-6c03-401f-aea3-9679888e409a"/>

4. 設定ができたら自動整形が機能するか、適当にファイルを作成してテストしてみましょう。デフォルトでは `Alt` + `Shift` + `F` で整形できます。

   ```cpp
   // 整形前
   void loop(){
   int a = 100;
   double b = 100.0;
   }
   ```

   ```cpp
   // 整形後
   void loop()
   {
       int a = 100;
       double b = 100.0;
   }
   ```



### C/C++ 拡張機能インストール

この拡張機能はインテリセンスの機能を持ちます。定義先に移動したり、関数名などをホバーすることで記載されている doxygen コメントを表示させたりできます。

[C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)

定義先に移動するには、c_cpp_properties.json に本ライブラリのインクルードパスを指定する必要があります。

```json
"${workspaceFolder}/src"
```

<img width="600px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/606eefda-83c1-4c6c-bd0a-7a2c5828fed9"/>

↓ `f12` で関数、クラス定義先へ移動できます。

<img width="400px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/48435e12-0893-4680-86eb-84d041c3f110"/>

↓ 関数、クラス名をホバーすることで、doxygen コメントが表示されます。

<img width="400px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/cc3597a8-1716-4ffb-91ff-395eb51f8d6b"/>

### スペルチェッカー拡張機能インストール

英単語のスペルミスを検出してくれるツールです。ミスがある場合、修正の提案もしてくれます。スペルチェッカーが知らない単語は辞書に追加することで対応できます。

[Code Spell Checker](https://marketplace.visualstudio.com/items?itemName=streetsidesoftware.code-spell-checker)

↓ スペルミス検出の様子

<img width="400px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/15563ebb-722d-4694-9666-8e4a61a2275f"/>

↓ 修正の提案

<img width="400px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/069a498d-cf93-4aba-a10e-00f068764469"/>

## Visual Studio

マイコンの機能に依存しないアルゴリズム系の開発の場合、デバッグのしやすい Visual Studio が便利です。最終的に完成したファイルをライブラリに追加し Arduino IDE でコンパイルできるか確認します。

### ライブラリを開く

本ライブラリは Visual Studio で開くのに必要なソリューションファイル (\*.sln) を持っていません。そのためライブラリごと開くことができません。

### 開発の流れ (1 から作成する場合)

1. Visual Studio で新規プロジェクトを作成し、任意のアルゴリズム等を実装
2. 実装したファイルを `UdonLibrary/src` 下にいい感じに分類して追加
3. Arduino 等で動作確認

### 開発の流れ (既存のファイルを変更する場合)

1. Visual Studio で新規プロジェクトを作成し、本ライブラリをインクルード
2. インクルード先に移動し、既存のファイルを編集
3. デバッグする
4. Arduino 等で動作確認

### clang-format

ツールタブからオプション選択しダイアログを開きます。以下の写真に示すように ClangFormat サポートの有効化にチェックを入れるだけ！

`Ctrl` + `K` → `Ctrl` + `D` で整形できます。

<img width="600px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/474dc5d1-5989-4c3c-bb9f-fdafbe64a810"/>

## GitHub Copilot 導入

GitHub Copilot とは AI がソースコードを提案してくれる便利な開発支援ツールです。使用方法によりますが、このツールを導入すると開発速度が数倍アップします。

有料ですが、学生は申請を行うことで無料で使用できるので、申請することをお勧めします。なくても開発は可能です。GitHub Copilot は GitHub Student Developer Pack に付属している GitHub Global Campus に入ると使用できるようになります。GitHub Student Developer Pack は他にも有用な有料ツールを無料で使用できるようになる特権があるため、こちらに申請することをお勧めします。[申請ページ](https://education.github.com/)

[VSCode へのインストール手順](https://docs.github.com/ja/copilot/using-github-copilot/getting-started-with-github-copilot#visual-studio-code-%E3%81%A7-github-copilot-%E6%8B%A1%E5%BC%B5%E6%A9%9F%E8%83%BD%E3%82%92%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%E3%81%99%E3%82%8B)

[Visual Studio へのインストール手順](https://learn.microsoft.com/ja-jp/visualstudio/ide/visual-studio-github-copilot-extension?view=vs-2022#install-using-the-visual-studio-installer)

↓ GitHub Copilot による自動補完の様子 (灰色文字)

<img width="400px" src="https://github.com/udonrobo/UdonLibrary/assets/91818705/c658acee-7941-4ea3-9eec-6390e85fda97"/>
