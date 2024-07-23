# UdonLibrary

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/53d063f8-ee25-41c9-b68b-75dcfd8ee5ad" width="40%" align="right"/>

香川高専高松機械システム研究部 C++ ライブラリ

[![Arduino Lint](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml)
[![Unit Tests](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml)
[![Doxygen Page](https://github.com/udonrobo/UdonLibrary/actions/workflows/DoxygenPage.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/DoxygenPage.yml)

| 系統             | ターゲット環境                                   |
| ---------------- | ------------------------------------------------ |
| Arduino          | Arduino Nano/Uno/UnoMini                         |
| Teensy           | Teensy 3.5/3.6/4.0/4.1                           |
| RP2040           | Raspberry Pi Pico/Pico W <br> Seeed XIAO RP2040  |
| Vanilla C++(11~) | Visual Studio <br> CLion <br> gcc, clang, etc... |

## 追加方法

- [端末/git](./docs/Install/RequiredTools.md)
- [Arduino IDE](./docs/Install/ArduinoIDE.md)
- [Platform IO](./docs/Install/PlatformIO.md)
- [Visual Studio](./docs/Install/VisualStudio.md)
- [CLion](./docs/Install/CLion.md)
- [Vanilla C++](./docs/Install/VanillaCpp.md)
- [Raspberry Pi Pico 使用時事前セットアップ](./docs/Install/RaspberryPiPico.md)

## 使用方法

### スタート

- [本ライブラリをインクルードする](./docs/Start/Include.md)

### アルゴリズム

- [ループ周期制御](./docs/Algorithm/LoopCycleController.md)
- [移動平均](./docs/Algorithm/MovingAverage.md)
- [フィードバック制御(PID)](./docs/Algorithm/FeedbackController.md)
- [独立ステア最適化](./docs/Algorithm/SteerOptimizer.md)
- [割り込み禁止制御](./docs/Algorithm/Interrupt.md)

### 通信

- [共通](./docs/Communication/Common.md)
- [CAN](./docs/Communication/CAN.md)
- [I2C](./docs/Communication/I2C.md)
- [LoRa](./docs/Communication/LoRa.md)
- [IM920](./docs/Communication/IM920.md)
- [USB/UART](./docs/Communication/Serial.md)
- [コントローラ](./docs/Communication/Pad.md)
- [CRC エラーチェック](./docs/Communication/CRC.md)

### ユーザー定義型

- [色空間](./docs/Types/Color.md)
- [二次元ベクトル](./docs/Types/Vector2D.md)
- [三次元ベクトル](./docs/Types/Vector3D.md)
- [オイラー角](./docs/Types/Eular.md)
- [クオータニオン](./docs/Types/Quaternion.md)
- [固定ビット長浮動小数点型](./docs/Types/Float.md)
- [コンテナ / 静的可変長配列](./docs/Types/StaticVector.md)
- [コンテナ / 静的可変長リングバッファ](./docs/Types/RingBuffer.md)
- [コンテナ / 配列ビュー](./docs/Types/ArrayView.md)

### ドライバー

- [モーター](./docs/Driver/Motor.md)
- [ロボマスモーター](./docs/Driver/RoboMasterMotor.md)
- [BNO055](./docs/Driver/BNO055.md)
- [DIP スイッチ](./docs/Driver/DipSwitch.md)
- [Raspberry Pi Pico / ロータリーエンコーダー](./docs/Driver/EncoderPico.md)
- [Raspberry Pi Pico / PIO](./docs/Driver/PIO.md)

### その他

- [シリアライザ・デシリアライザ](./docs/Other/Serialization.md)
- [メタ関数](./docs/Other/Traits.md)

### 開発者用

- [開発環境セットアップ](./docs/Developer/DevelopSetup.md)
- [スタイルガイド](./docs/Developer/StyleGuide.md)
- [ディレクトリ構造](./docs/Developer/DirectoryStructure.md)
- [GitHub Actions / 自動テスト](./docs/Developer/CI.md)
- [GitHub Actions / ドキュメント自動生成](./docs/Developer/DoxygenPage.md)
- [謝辞](./docs/Developer/Acknowledgments.md)

## ライセンス

本ライブラリは GPL-2.0 ライセンスの下で公開されています。将来的に MIT ライセンスに移行予定です。
