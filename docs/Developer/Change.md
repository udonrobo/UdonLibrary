# 変更・追加方法

ライブラリをエディターで開きます。

```sh
code ~/Documents/Arduino/libraries/UdonLibrary/
```

![image](https://github.com/user-attachments/assets/c3cd152e-375f-4cb5-a1c7-babeb3a97bed)

## ブランチを切る (任意)

ソース管理タブへ移動し、作業用ブランチを作成します。急ぎで修正の場合、エラーにほぼ確定でならないような修正の場合はメインのブランチのままで大丈夫です。

作業用ブランチで作業すると、やらかした場合にブランチごと消せばもとに戻せるため楽です。

![image](https://github.com/user-attachments/assets/1c85b075-36fc-4b20-abe0-3c525de10c88)

![image](https://github.com/user-attachments/assets/7f647c01-fe5b-41f9-87bf-b9b8cb889b3c)

ブランチが作成できると、左下に今いるブランチが表示されます。ブランチを移動したい場合はここを押し、移動先のブランチを選びます。

![image](https://github.com/user-attachments/assets/7d8c2c30-c99e-4df1-8784-01991b5d4cc8)

## 変更のアップロード

変更し、コミットします。きりが付いたら GitHub へアップロード

![image](https://github.com/user-attachments/assets/1ca69ffd-8b58-4d55-aade-60c2f97afea3)

## 自動テストの確認

GitHub Actions によってテストが動くので、エラーになっていないか確認します。ローカル PC でもテストは実行できます。

![image](https://github.com/user-attachments/assets/49e26eb2-c847-4000-8906-93e4212701ab)

エラーになっている場合、そのテストを選択すると、エラーメッセージを見れます。修正して再度 GitHub へアップロードします。

![image](https://github.com/user-attachments/assets/c1862822-43b5-44b2-be73-3092f1afd36a)

## Pull Request 作成 (ブランチを切った場合)

テストを通過したら、Pull Request を作成し、メインのブランチにマージします。

![image](https://github.com/user-attachments/assets/d7858cfa-ccae-49d2-bf2d-fd328cbce1cb)

どのブランチをどのブランチへマージするか聞かれるので、開発用ブランチをメインのブランチへマージするようにします。

![image](https://github.com/user-attachments/assets/89de849d-d8bd-4970-9f22-0d72f8deb91e)

どのような変更をしたのかを記入し、Pull Request を作成します。

![image](https://github.com/user-attachments/assets/f6bc4454-d9ac-48ff-adf2-45155eb4813f)

マージします。テストが再度走りますが、前回のテストを通過していれば大抵通ります。

![image](https://github.com/user-attachments/assets/56105475-92ea-459d-93fb-caa878c5dabb)

![image](https://github.com/user-attachments/assets/3f5a28f7-4245-48ca-b36d-01814a9f931e)

![image](https://github.com/user-attachments/assets/d2f3cfd1-1cbe-4606-98a8-d6fd67c0e40f)

開発用ブランチは不要なので削除します。

![image](https://github.com/user-attachments/assets/4ec5c75a-eae1-4063-ab6c-504f7db46757)

VSCode に戻り、メインのブランチへ移動します。ソース管理タブの「変更を取り込む」を押し、変更がメインのブランチに反映されたか確認します。他の制御班にも変更を取り込んでもらいましょう！

完了👍👍
