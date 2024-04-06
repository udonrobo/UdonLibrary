# UdonLibrary [![Arduino Lint](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/ArduinoLint.yml) [![Unit Tests](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml/badge.svg)](https://github.com/udonrobo/UdonLibrary/actions/workflows/UnitTest.yml)

<img src="https://github.com/udonrobo/UdonLibrary/assets/91818705/53d063f8-ee25-41c9-b68b-75dcfd8ee5ad" height="300px" align="right"/>

香川高専高松機械システム研究部 C++ ライブラリ

## 追加方法

- [Arduino IDE](./docs/Install/ArduinoIDE.md)
- [Platform IO](./docs/Install/PlatformIO.md)
- [Visual Studio](./docs/Install/VisualStudio.md)
- [Visual Studio (submodule)](./docs/Install/VisualStudioSubmodule.md)
- [Raspberry Pi Pico 使用時事前セットアップ](./docs/Install/RaspberryPiPico.md)

## 使用方法

### スタート

- [本ライブラリをインクルードする](./docs/Start/Include.md)

### アルゴリズム

- [ループ周期制御](./docs/Algorithm/LoopCycleController.md)🌟new
- [移動平均](./docs/Algorithm/MovingAverage.md)🌟new
- [フィードバック制御(PID)](./docs/Algorithm/FeedbackController.md)🌟new
- [独立ステア最適化](./docs/Algorithm/SteerOptimizer.md)

### 通信

- [共通](./docs/Communication/Common.md)
- [CAN](./docs/Communication/CAN.md)
- [I2C](./docs/Communication/I2C.md)
- [UART](./docs/Communication/UART.md)
- [LoRa](./docs/Communication/LoRa.md)🌟new
- [IM920](./docs/Communication/IM920.md)
- [コントローラ](./docs/Communication/Pad.md)🌟new

### ユーザー定義型

- [色空間](./docs/Types/Color.md)
- [二次元ベクトル](./docs/Types/Vector2D.md)
- [三次元ベクトル](./docs/Types/Vector3D.md)
- [オイラー角](./docs/Types/Eular.md)
- [クオータニオン](./docs/Types/Quaternion.md)
- [コンテナ / 静的可変長配列](./docs/Types/StaticVector.md)
- [コンテナ / 静的可変長リングバッファ](./docs/Types/RingBuffer.md)
- [コンテナ / 配列ビュー](./docs/Types/ArrayView.md)
- [固定ビット長浮動小数点型](./docs/Types/Float.md)

### ドライバー

- [モーター](./docs/Driver/Motor.md)
- [ロボマスモーター](./docs/Driver/RoboMasterMotor.md)🌟new
- [ロータリーエンコーダー](./docs/Driver/Encoder.md)

### その他

- [シリアライザ・デシリアライザ](./docs/Other/Serialization.md)
- [トレイト・メタ関数](./docs/Other/Traits.md)

### 開発者用

- [開発環境セットアップ](./docs/Developer/DevelopSetup.md)🌟new
- [スタイルガイド](./docs/Developer/StyleGuide.md)
- [ディレクトリ構造](./docs/Developer/DirectoryStructure.md)
- [自動テスト](./docs/Developer/CI.md)
- [謝辞](./docs/Developer/Acknowledgments.md)
