# ドキュメント自動生成

Doxygen を用いてソースコード、マークダウンからドキュメントを自動生成しています。マスターブランチにプッシュがあった場合 GitHub Actions を通じてページが更新されます。ワークフローは `.github/workflows/DoxygenPage.yml` に定義されており、以下の流れで更新を行います。

```mermaid
flowchart TB

    onpush{{プッシュがあった時}} --> DoxygenPageフロー

    subgraph DoxygenPageフロー
        direction TB
        DoxygenがHTMLページを生成 --> 生成物をgh_pagesブランチにプッシュ
    end

    DoxygenPageフロー --> デプロイフロー

    subgraph デプロイフロー
        gh_pagesブランチのHTMLページを公開
    end
```

## ローカル環境で実行

1. Doxygen をインストール

   <https://www.doxygen.nl/download.html#latestsrc>

   Doxygen コマンドが使用できるようパスを通す必要があります。

2. UdonLibrary ディレクトリで Doxygen を実行

   ```sh
   doxygen Doxyfile
   ```

3. ページ が ./docs/DoxygenDist/html に生成されるのでブラウザで開く

   ```sh
   start ./docs/DoxygenDist/html/index.html
   ```

   > 生成物は git に追跡されません。

## Doxygen のメリット

ソースコード中にドキュメントを記述するため、コードの変更に追従しやすく、最新の情報を提供できる。

クラス一覧、関数一覧、ファイル一覧、変数一覧などを自動生成できる。

![image](https://github.com/user-attachments/assets/5c0de23f-40f5-4eba-bf0f-81a088911515)

クラスの継承関係、関数の呼び出し関係などをグラフで表示できる。

![image](https://github.com/user-attachments/assets/9ae676e7-256b-4583-9b93-dae7ebe64ef3)

マークダウンファイルもドキュメントとして生成でき、コードブロック等に記述したコードのシンボル名を自動的にソースコードとリンクしてくれる

![image](https://github.com/user-attachments/assets/567a69a6-ad38-4116-be2b-e1bb66213fc8)
