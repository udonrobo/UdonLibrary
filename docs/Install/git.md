# git のインストール

本ライブラリはバージョン管理に git を使用します。インストールしていない場合はインストールしてください。

## 公式ページからインストール

<https://git-scm.com/download>

## ターミナルからインストール

winget (Windows)

```sh
winget install --id Git.Git -e --source winget
```

apt-get (Linux)

```sh
sudo apt-get update && sudo apt-get install git-all
```

Homebrew (macOS or Linux)

```sh
brew install git
```

> 本ライブラリのレポジトリはプライベートであるため、クローンするには udonrobo に 参加している GitHub アカウントと、 git が紐付いている必要があります。git インストール後、初回のクローン時に紐付けを求められます。
