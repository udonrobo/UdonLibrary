# UdonLibrary

## 概要

香川高専高松機械システム研究部 Arduino ライブラリ

## ライブラリ管理

-   git

    ライブラリ管理に git を使用します。インストールしていない場合は [公式ページ](https://git-scm.com/download/win) からダウンロードし、インストールしてください。

    このライブラリのレポジトリはプライベートです。クローンするには udonrobo organization に 参加している github アカウントと、 git が紐付いている必要があります。git インストール後、初回のクローン時に紐付けを求められます。

-   追加

    ライブラリを保存するディレクトリを調べライブラリを追加します。

    > 既定値: `~\Documents\Arduino\libraries`
    >
    > 変更している場合: `ファイル > 環境設定 > スケッチブックの保存場所欄` + `\libraries`

    > ```sh
    > # 調べたディレクトリに移動
    > cd ~\Documents\Arduino\libraries
    >
    > # ライブラリをクローン
    > git clone https://github.com/udonrobo/Library.git
    > ```

-   更新

    > ```sh
    > # 追加する時に調べたlibrariesディレクトリに移動
    > cd ~\Documents\Arduino\libraries
    >
    > # ライブラリを更新
    > git pull
    > ```

## 機能一覧

-   通信



## ディレクトリ構造

-   dev

    スケッチ例、開発用ディレクトリ

-   src

    ライブラリのヘッダー、ソースファイル群
