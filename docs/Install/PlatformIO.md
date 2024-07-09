# Platform IO

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/b00cf135-38be-4b9b-97e6-0d5de337d1a3" height="100px" align="right">

1. `platformio.ini` を開きライブラリの URL を設定します。

   ```
   lib_deps =
       https://github.com/udonrobo/UdonLibrary.git
   ```

   本ライブラリは C++ 標準ライブラリに依存しています。標準ライブラリが使用できない環境 (Arduino Nano 等) で使用するには UdonArduinoSTL を合わせて追加してください。
   
   ```
   lib_deps =
       https://github.com/udonrobo/UdonLibrary.git
       https://github.com/udonrobo/UdonArduinoSTL.git
   ```

   ![image](https://github.com/udonrobo/UdonLibrary/assets/91818705/532fd53b-4dbf-41cf-b4e1-5d512f409dd2)

