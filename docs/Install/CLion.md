# CLion

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/68f91d1f-6d23-4650-b364-bcb94362e48d" height="100px" align="right">

## 追加

1. ターミナルをプロジェクトディレクトリ下で起動します。

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/2dfb4e37-23cc-42dc-a0ff-4e6fc3743a3e" >

2. ライブラリをクローンします。

   ```sh
   git clone --recursive https://github.com/udonrobo/UdonLibrary.git
   ```

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/6c9fbe6b-3368-4a20-a1f9-97905f9e54ce" >

3. CMakeLists.txt にインクルードパスを設定します。

   \<target\> には `add_executable(...)` によって作成されるターゲット名を指定します。

   ```cmake
   target_include_directories( <target> PUBLIC ./UdonLibrary/src)
   ```

   <img width=700px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/6fbdd219-12b9-425e-b7ed-6bbcf02ba42b" >

## 更新

1. `追加 1.` と同じくターミナルを起動します。

2. ライブラリディレクトリに移動しプルします。

   ```sh
   cd UdonLibrary
   git pull
   ```

   <img width=600px src="https://github.com/udonrobo/UdonLibrary/assets/91818705/eeec6cb9-9d31-412a-a8ad-98bc085e70ce" >
