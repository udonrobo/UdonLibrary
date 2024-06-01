# CLion

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/68f91d1f-6d23-4650-b364-bcb94362e48d" height="100px" align="right">

以下の手順の追加を行います

1. ライブラリのクローン
2. インクルードパスの設定

## 追加

1. ターミナルをプロジェクトディレクトリ下で起動

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/2dfb4e37-23cc-42dc-a0ff-4e6fc3743a3e" >

2. ライブラリをクローン

   - プロジェクトを git で管理していない場合

     ```
     git clone --recursive https://github.com/udonrobo/UdonLibrary.git
     ```

      <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/6c9fbe6b-3368-4a20-a1f9-97905f9e54ce" >

   - プロジェクトを git で管理している場合

     リポジトリ内にリポジトリを置くことになります。この場合、submodule として追加することが推奨されています。

     submodule add 後、追加先のレポジトリに対して、「ライブラリを追加した」という変更が自動的にステージングされるので、コミットを作成しています。

     ```
     git submodule add https://github.com/udonrobo/UdonLibrary.git
     git commit -m "add UdonLibrary"
     ```

     <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/f87a5658-d291-48c3-80d5-e9ff739d6690" >

3. CMakeLists.txt にインクルードパスを設定

   \<target\> には `add_executable(...)` によって作成されるターゲット名を指定します。

   ```cmake
   target_include_directories( <target> PUBLIC ./UdonLibrary/src)
   ```

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/6fbdd219-12b9-425e-b7ed-6bbcf02ba42b" >

4. 追加確認

   追加が成功している場合、以下の様に Udon.hpp のインクルードができるようになっています。

   <img width=400px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/253323fd-9d80-49e0-99ed-17e0de1d3e09">

## 更新

1. `追加 1.` と同じくターミナルを起動

2. 更新

   - プロジェクトを git で管理していない場合

     ライブラリディレクトリに移動しプルします。

     ```
     cd UdonLibrary
     git pull
     ```

     <img width=600px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/eeec6cb9-9d31-412a-a8ad-98bc085e70ce" >

   - プロジェクトを git で管理している場合

     追加先のプロジェクトでサブモジュールを更新します。

     ```
     git submodule update --remote
     ```

     <img width=600px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/dc1fecbb-2a66-402b-a3dc-5994663920e5" >

## submodule 使用時の注意点

追加先のプロジェクト自体のクローンを行うとき `--recursive` オプションを与えなければ、ライブラリがクローンされません。

```
git clone --recursive <プロジェクトURL>
```

また submodule は追加時のコミットを参照するため、追加先のプロジェクトをクローンしても、ライブラリは submodule 追加時のコミットの内容になります。最新のライブラリを使用する場合は submodule を更新する必要があります。

```
git submodule update --remote
```
