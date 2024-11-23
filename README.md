# UdonLibrary

<img src="https://github.com/user-attachments/assets/e00694d1-9eb0-4178-8843-b18e86f1678a" width="40%" align="right"/>

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

- [Arduino IDE](./docs/Install/ArduinoIDE.md)
- [Platform IO](./docs/Install/PlatformIO.md)
- [Visual Studio](./docs/Install/VisualStudio.md)
- [CLion](./docs/Install/CLion.md)
- [バニラ C++](./docs/Install/VanillaCpp.md)
- [Raspberry Pi Pico 使用時事前セットアップ](./docs/Install/RaspberryPiPico.md)

## 使用方法

### スタート

- [本ライブラリをインクルードする](./docs/Start/Include.md)

### アルゴリズム

- [ループ周期制御](./docs/Algorithm/LoopCycleController.md)
- [移動平均](./docs/Algorithm/MovingAverage.md)
- [PID 制御](./docs/Algorithm/FeedbackController.md)
- [独立ステア最適化](./docs/Algorithm/SteerOptimizer.md)
- [割り込み禁止制御](./docs/Algorithm/Interrupt.md)

### 通信

- [共通](./docs/Communication/Common.md)
- [メッセージ型](./docs/Communication/Message.md)
- [CAN](./docs/Communication/CAN.md)
- [I2C](./docs/Communication/I2C.md)
- [LoRa](./docs/Communication/LoRa.md)
- [IM920 (動作不明)](./docs/Communication/IM920.md)
- [USB/UART](./docs/Communication/Serial.md)
- [コントローラー](./docs/Communication/Pad.md)
- [コントローラーを構成する](./docs/Communication/PadOrganization.md)
- [CRC エラーチェック](./docs/Communication/CRC.md)

### ドライバー

- [モーター](./docs/Driver/Motor.md)
- [ロボマスモーター](./docs/Driver/RoboMasterMotor.md)
- [BNO055](./docs/Driver/BNO055.md)
- [DIP スイッチ](./docs/Driver/DipSwitch.md)
- [7 セグメント LED](./docs/Driver/SegmentsLed.md)
- [Raspberry Pi Pico / PIO](./docs/Driver/PIO.md)
- [Raspberry Pi Pico / サーボ](./docs/Driver/PicoServo.md)
- [Raspberry Pi Pico / ロータリーエンコーダー](./docs/Driver/EncoderPico.md)
- [Raspberry Pi Pico / ウォッチドックタイマー](./docs/Driver/PicoWDT.md)

### ユーザー定義型

- [色空間](./docs/Types/Color.md)
- [Optional 型](./docs/Types/Optional.md)
- [二次元ベクトル型](./docs/Types/Vector2D.md)
- [三次元ベクトル型](./docs/Types/Vector3D.md)
- [オイラー角型](./docs/Types/Eular.md)
- [クオータニオン型](./docs/Types/Quaternion.md)
- [固定ビット長浮動小数点型](./docs/Types/Float.md)
- [コンテナ / 静的可変長配列](./docs/Types/StaticVector.md)
- [コンテナ / 静的可変長リングバッファ](./docs/Types/RingBuffer.md)
- [コンテナ / 配列ビュー](./docs/Types/ArrayView.md)

### その他

- [ユーティリティ](./docs/Other/Utility.md)
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

本ライブラリは GPL-2.0 ライセンスの下で公開されています。
