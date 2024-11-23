/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "UdonLibrary", "index.html", [
    [ "追加方法", "index.html#autotoc_md1", null ],
    [ "使用方法", "index.html#autotoc_md2", [
      [ "スタート", "index.html#autotoc_md3", null ],
      [ "アルゴリズム", "index.html#autotoc_md4", null ],
      [ "通信", "index.html#autotoc_md5", null ],
      [ "ドライバー", "index.html#autotoc_md6", null ],
      [ "ユーザー定義型", "index.html#autotoc_md7", null ],
      [ "その他", "index.html#autotoc_md8", null ],
      [ "開発者用", "index.html#autotoc_md9", null ]
    ] ],
    [ "ライセンス", "index.html#autotoc_md10", null ],
    [ "フィードバック制御", "md_docs_2Algorithm_2FeedbackController.html", [
      [ "PidController", "md_docs_2Algorithm_2FeedbackController.html#autotoc_md12", null ]
    ] ],
    [ "割り込み禁止制御", "md_docs_2Algorithm_2Interrupt.html", null ],
    [ "ループ周期制御", "md_docs_2Algorithm_2LoopCycleController.html", null ],
    [ "移動平均", "md_docs_2Algorithm_2MovingAverage.html", null ],
    [ "独立ステア最適化", "md_docs_2Algorithm_2SteerOptimizer.html", [
      [ "個別インクルード", "md_docs_2Algorithm_2SteerOptimizer.html#autotoc_md17", null ],
      [ "全車輪最適化", "md_docs_2Algorithm_2SteerOptimizer.html#autotoc_md18", null ],
      [ "車輪ごとに最適化", "md_docs_2Algorithm_2SteerOptimizer.html#autotoc_md19", null ],
      [ "Sample", "md_docs_2Algorithm_2SteerOptimizer.html#autotoc_md20", null ],
      [ "OpenSiv3D でシミュレートする", "md_docs_2Algorithm_2SteerOptimizer.html#autotoc_md21", null ]
    ] ],
    [ "CAN 通信", "md_docs_2Communication_2CAN.html", [
      [ "個別インクルード", "md_docs_2Communication_2CAN.html#autotoc_md23", null ],
      [ "通信バスクラス", "md_docs_2Communication_2CAN.html#autotoc_md24", [
        [ "■ Teensy", "md_docs_2Communication_2CAN.html#autotoc_md25", null ],
        [ "■ Raspberry Pi Pico", "md_docs_2Communication_2CAN.html#autotoc_md26", null ]
      ] ],
      [ "送信ノードクラス", "md_docs_2Communication_2CAN.html#autotoc_md27", null ],
      [ "受信ノードクラス", "md_docs_2Communication_2CAN.html#autotoc_md28", null ],
      [ "バイト列を直接送受信", "md_docs_2Communication_2CAN.html#autotoc_md29", [
        [ "■ 送信ノード", "md_docs_2Communication_2CAN.html#autotoc_md30", null ],
        [ "■ 受信ノード", "md_docs_2Communication_2CAN.html#autotoc_md31", null ]
      ] ],
      [ "デバッグ", "md_docs_2Communication_2CAN.html#autotoc_md32", null ],
      [ "クラスの組み合わせ色々", "md_docs_2Communication_2CAN.html#autotoc_md33", null ]
    ] ],
    [ "通信共通部", "md_docs_2Communication_2Common.html", [
      [ "共通内容", "md_docs_2Communication_2Common.html#autotoc_md35", null ],
      [ "送信者イメージ", "md_docs_2Communication_2Common.html#autotoc_md36", null ],
      [ "受信者イメージ", "md_docs_2Communication_2Common.html#autotoc_md37", null ],
      [ "複数の通信バスや送受信者が存在する場合", "md_docs_2Communication_2Common.html#autotoc_md38", [
        [ "複数送信者", "md_docs_2Communication_2Common.html#autotoc_md39", null ],
        [ "送受信者", "md_docs_2Communication_2Common.html#autotoc_md40", null ],
        [ "複数バス、複数送受信", "md_docs_2Communication_2Common.html#autotoc_md41", null ]
      ] ]
    ] ],
    [ "CRC エラーチェック", "md_docs_2Communication_2CRC.html", [
      [ "Udon::CRC8", "md_docs_2Communication_2CRC.html#autotoc_md43", null ],
      [ "CRC の概要", "md_docs_2Communication_2CRC.html#autotoc_md44", null ]
    ] ],
    [ "I2C 通信", "md_docs_2Communication_2I2C.html", [
      [ "個別インクルード", "md_docs_2Communication_2I2C.html#autotoc_md46", null ],
      [ "バスクラス", "md_docs_2Communication_2I2C.html#autotoc_md47", null ],
      [ "マスターからスレーブへ送信", "md_docs_2Communication_2I2C.html#autotoc_md48", [
        [ "マスター送信クラス", "md_docs_2Communication_2I2C.html#autotoc_md49", null ],
        [ "スレーブ受信クラス", "md_docs_2Communication_2I2C.html#autotoc_md50", null ]
      ] ],
      [ "スレーブからマスターへ送信", "md_docs_2Communication_2I2C.html#autotoc_md51", [
        [ "スレーブ送信クラス", "md_docs_2Communication_2I2C.html#autotoc_md52", null ],
        [ "マスター受信クラス", "md_docs_2Communication_2I2C.html#autotoc_md53", null ]
      ] ]
    ] ],
    [ "IM920", "md_docs_2Communication_2IM920.html", [
      [ "個別インクルード", "md_docs_2Communication_2IM920.html#autotoc_md57", null ],
      [ "Usage", "md_docs_2Communication_2IM920.html#autotoc_md58", null ],
      [ "Im920 クラス", "md_docs_2Communication_2IM920.html#autotoc_md59", null ],
      [ "送信クラス", "md_docs_2Communication_2IM920.html#autotoc_md60", null ],
      [ "受信クラス", "md_docs_2Communication_2IM920.html#autotoc_md61", null ]
    ] ],
    [ "LoRa", "md_docs_2Communication_2LoRa.html", [
      [ "使用部品", "md_docs_2Communication_2LoRa.html#autotoc_md63", null ],
      [ "E220 LoRa モジュール", "md_docs_2Communication_2LoRa.html#autotoc_md64", [
        [ "スケッチ例 / 受信側", "md_docs_2Communication_2LoRa.html#autotoc_md65", null ],
        [ "スケッチ例 / 送信側", "md_docs_2Communication_2LoRa.html#autotoc_md66", null ],
        [ "チャンネルの設定", "md_docs_2Communication_2LoRa.html#autotoc_md67", null ],
        [ "その他設定値の変更", "md_docs_2Communication_2LoRa.html#autotoc_md68", null ],
        [ "受信強度", "md_docs_2Communication_2LoRa.html#autotoc_md69", null ],
        [ "周波数", "md_docs_2Communication_2LoRa.html#autotoc_md70", null ]
      ] ]
    ] ],
    [ "メッセージ型", "md_docs_2Communication_2Message.html", [
      [ "メッセージ型一覧", "md_docs_2Communication_2Message.html#autotoc_md72", null ],
      [ "新たにメッセージ型を定義する", "md_docs_2Communication_2Message.html#autotoc_md73", null ]
    ] ],
    [ "PS5 コントローラー", "md_docs_2Communication_2Pad.html", [
      [ "個別インクルード", "md_docs_2Communication_2Pad.html#autotoc_md75", null ],
      [ "インスタンス化", "md_docs_2Communication_2Pad.html#autotoc_md76", null ],
      [ "通信開始 (Optional)", "md_docs_2Communication_2Pad.html#autotoc_md77", null ],
      [ "更新 (必須)", "md_docs_2Communication_2Pad.html#autotoc_md78", null ],
      [ "遠隔非常停止の実装", "md_docs_2Communication_2Pad.html#autotoc_md79", null ],
      [ "ボタン", "md_docs_2Communication_2Pad.html#autotoc_md80", null ],
      [ "スティック", "md_docs_2Communication_2Pad.html#autotoc_md81", null ],
      [ "最終的なスケッチ例 (CAN バス経由)", "md_docs_2Communication_2Pad.html#autotoc_md82", null ],
      [ "メインマイコンと USB ホストシールドが直接接続されている場合", "md_docs_2Communication_2Pad.html#autotoc_md83", null ]
    ] ],
    [ "コントローラーの構成", "md_docs_2Communication_2PadOrganization.html", [
      [ "構成を考える", "md_docs_2Communication_2PadOrganization.html#autotoc_md85", [
        [ "■ 920MHz 経由 / 無線モジュールがメイン基板にある場合 (推奨)", "md_docs_2Communication_2PadOrganization.html#autotoc_md86", null ],
        [ "■ 920MHz 経由 / 無線モジュールとメイン基板が別々の場合 (推奨)", "md_docs_2Communication_2PadOrganization.html#autotoc_md87", null ],
        [ "■ Bluetooth 経由 / USB ホストシールドがメイン基板にある場合", "md_docs_2Communication_2PadOrganization.html#autotoc_md88", null ],
        [ "■ Bluetooth 経由 / USB ホストシールドとメイン基板が別々の場合", "md_docs_2Communication_2PadOrganization.html#autotoc_md89", null ],
        [ "■ USB 経由", "md_docs_2Communication_2PadOrganization.html#autotoc_md90", null ]
      ] ],
      [ "送信側マイコン用クラス", "md_docs_2Communication_2PadOrganization.html#autotoc_md91", null ],
      [ "受信側マイコン用クラス", "md_docs_2Communication_2PadOrganization.html#autotoc_md92", null ],
      [ "Bluetooth 使用時の受信側マイコン用クラス", "md_docs_2Communication_2PadOrganization.html#autotoc_md93", null ],
      [ "Bluetooth ドングルとのペアリング", "md_docs_2Communication_2PadOrganization.html#autotoc_md94", null ]
    ] ],
    [ "UART / USB Serial", "md_docs_2Communication_2Serial.html", [
      [ "マイコンから PC", "md_docs_2Communication_2Serial.html#autotoc_md96", [
        [ "マイコン側", "md_docs_2Communication_2Serial.html#autotoc_md97", null ],
        [ "PC 側 (OpenSiv3D が必要)", "md_docs_2Communication_2Serial.html#autotoc_md98", null ]
      ] ],
      [ "PC からマイコン", "md_docs_2Communication_2Serial.html#autotoc_md99", [
        [ "PC 側 (OpenSiv3D が必要)", "md_docs_2Communication_2Serial.html#autotoc_md100", null ],
        [ "マイコン側", "md_docs_2Communication_2Serial.html#autotoc_md101", null ]
      ] ],
      [ "OpenSiv3D COMポート列挙方法", "md_docs_2Communication_2Serial.html#autotoc_md102", null ]
    ] ],
    [ "謝辞", "md_docs_2Developer_2Acknowledgments.html", null ],
    [ "自動テスト", "md_docs_2Developer_2CI.html", [
      [ "Arduino Lint", "md_docs_2Developer_2CI.html#autotoc_md105", [
        [ "■ 概要", "md_docs_2Developer_2CI.html#autotoc_md106", null ],
        [ "■ ディレクトリを追加する", "md_docs_2Developer_2CI.html#autotoc_md107", null ],
        [ "■ テストファイルを追加する", "md_docs_2Developer_2CI.html#autotoc_md108", null ],
        [ "■ コンパイル時に計算可能なアルゴリズムのテスト", "md_docs_2Developer_2CI.html#autotoc_md109", null ],
        [ "■ 検証ボードを追加する", "md_docs_2Developer_2CI.html#autotoc_md110", null ],
        [ "■ ローカル環境で実行", "md_docs_2Developer_2CI.html#autotoc_md111", null ]
      ] ],
      [ "Google Unit Test", "md_docs_2Developer_2CI.html#autotoc_md112", [
        [ "■ 概要", "md_docs_2Developer_2CI.html#autotoc_md113", null ],
        [ "■ ディレクトリを追加する", "md_docs_2Developer_2CI.html#autotoc_md114", null ],
        [ "■ ソースファイルを追加する", "md_docs_2Developer_2CI.html#autotoc_md115", null ],
        [ "■ テストを書く", "md_docs_2Developer_2CI.html#autotoc_md116", null ],
        [ "■ ローカル環境で実行", "md_docs_2Developer_2CI.html#autotoc_md117", null ]
      ] ]
    ] ],
    [ "開発環境セットアップ", "md_docs_2Developer_2DevelopSetup.html", [
      [ "VSCode", "md_docs_2Developer_2DevelopSetup.html#autotoc_md119", [
        [ "ライブラリを開く", "md_docs_2Developer_2DevelopSetup.html#autotoc_md120", null ],
        [ "clang-format 拡張機能インストール", "md_docs_2Developer_2DevelopSetup.html#autotoc_md121", null ],
        [ "C/C++ 拡張機能インストール", "md_docs_2Developer_2DevelopSetup.html#autotoc_md122", null ],
        [ "スペルチェッカー拡張機能インストール", "md_docs_2Developer_2DevelopSetup.html#autotoc_md123", null ]
      ] ],
      [ "Visual Studio", "md_docs_2Developer_2DevelopSetup.html#autotoc_md124", [
        [ "ライブラリを開く", "md_docs_2Developer_2DevelopSetup.html#autotoc_md125", null ],
        [ "開発の流れ (1 から作成する場合)", "md_docs_2Developer_2DevelopSetup.html#autotoc_md126", null ],
        [ "開発の流れ (既存のファイルを変更する場合)", "md_docs_2Developer_2DevelopSetup.html#autotoc_md127", null ],
        [ "clang-format", "md_docs_2Developer_2DevelopSetup.html#autotoc_md128", null ]
      ] ],
      [ "GitHub Copilot 導入", "md_docs_2Developer_2DevelopSetup.html#autotoc_md129", null ]
    ] ],
    [ "ディレクトリ構造", "md_docs_2Developer_2DirectoryStructure.html", null ],
    [ "ドキュメント自動生成", "md_docs_2Developer_2DoxygenPage.html", [
      [ "Doxygen コメントの書き方", "md_docs_2Developer_2DoxygenPage.html#autotoc_md132", null ],
      [ "ローカル環境で実行", "md_docs_2Developer_2DoxygenPage.html#autotoc_md133", null ],
      [ "Doxygen のメリット", "md_docs_2Developer_2DoxygenPage.html#autotoc_md134", null ]
    ] ],
    [ "スタイルガイド", "md_docs_2Developer_2StyleGuide.html", [
      [ "言語", "md_docs_2Developer_2StyleGuide.html#autotoc_md136", null ],
      [ "書式", "md_docs_2Developer_2StyleGuide.html#autotoc_md137", null ],
      [ "命名規則", "md_docs_2Developer_2StyleGuide.html#autotoc_md138", [
        [ "ディレクトリ(フォルダ)、ファイル", "md_docs_2Developer_2StyleGuide.html#autotoc_md139", null ],
        [ "変数 定数", "md_docs_2Developer_2StyleGuide.html#autotoc_md140", null ],
        [ "関数", "md_docs_2Developer_2StyleGuide.html#autotoc_md141", null ],
        [ "型", "md_docs_2Developer_2StyleGuide.html#autotoc_md142", null ],
        [ "テンプレート", "md_docs_2Developer_2StyleGuide.html#autotoc_md143", null ],
        [ "名前空間", "md_docs_2Developer_2StyleGuide.html#autotoc_md144", null ],
        [ "マクロ", "md_docs_2Developer_2StyleGuide.html#autotoc_md145", null ]
      ] ]
    ] ],
    [ "BNO055", "md_docs_2Driver_2BNO055.html", null ],
    [ "ディップスイッチ", "md_docs_2Driver_2DipSwitch.html", [
      [ "個別インクルード", "md_docs_2Driver_2DipSwitch.html#autotoc_md148", null ],
      [ "値読み取り", "md_docs_2Driver_2DipSwitch.html#autotoc_md149", null ]
    ] ],
    [ "エンコーダー", "md_docs_2Driver_2EncoderPico.html", [
      [ "個別インクルード", "md_docs_2Driver_2EncoderPico.html#autotoc_md151", null ],
      [ "スケッチ例", "md_docs_2Driver_2EncoderPico.html#autotoc_md152", null ]
    ] ],
    [ "モーター", "md_docs_2Driver_2Motor.html", [
      [ "個別インクルード", "md_docs_2Driver_2Motor.html#autotoc_md154", null ],
      [ "Motor2 クラス", "md_docs_2Driver_2Motor.html#autotoc_md155", null ],
      [ "Motor3 クラス", "md_docs_2Driver_2Motor.html#autotoc_md156", null ],
      [ "デバッグ", "md_docs_2Driver_2Motor.html#autotoc_md157", null ],
      [ "その他", "md_docs_2Driver_2Motor.html#autotoc_md158", null ]
    ] ],
    [ "サーボ", "md_docs_2Driver_2PicoServo.html", [
      [ "個別インクルード", "md_docs_2Driver_2PicoServo.html#autotoc_md160", null ],
      [ "スケッチ例", "md_docs_2Driver_2PicoServo.html#autotoc_md161", null ]
    ] ],
    [ "PicoWDT", "md_docs_2Driver_2PicoWDT.html", null ],
    [ "PIO", "md_docs_2Driver_2PIO.html", [
      [ "PioClockBegin", "md_docs_2Driver_2PIO.html#autotoc_md164", null ],
      [ "AllocateStateMachine", "md_docs_2Driver_2PIO.html#autotoc_md165", null ]
    ] ],
    [ "ロボマスモーター", "md_docs_2Driver_2RoboMasterMotor.html", [
      [ "個別インクルード", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md167", null ],
      [ "概要", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md168", null ],
      [ "電流値の指定", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md169", null ],
      [ "センサー値取得", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md170", null ],
      [ "複数モーターの場合", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md171", null ],
      [ "複数 CAN バスの場合", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md172", null ],
      [ "Raspberry Pi Pico での使用例", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md173", null ],
      [ "スケッチ例 (電流制御)", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md174", null ],
      [ "スケッチ例 (速度フィードバック制御)", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md175", null ],
      [ "スケッチ例 (速度制御クラスの作成例)", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md176", null ]
    ] ],
    [ "7 セグメント LED", "md_docs_2Driver_2SegmentsLed.html", [
      [ "個別インクルード", "md_docs_2Driver_2SegmentsLed.html#autotoc_md178", null ],
      [ "インスタンス化", "md_docs_2Driver_2SegmentsLed.html#autotoc_md179", null ],
      [ "値表示", "md_docs_2Driver_2SegmentsLed.html#autotoc_md180", null ],
      [ "無効値表示", "md_docs_2Driver_2SegmentsLed.html#autotoc_md181", null ]
    ] ],
    [ "Arduino IDE", "md_docs_2Install_2ArduinoIDE.html", [
      [ "追加", "md_docs_2Install_2ArduinoIDE.html#autotoc_md183", null ],
      [ "更新", "md_docs_2Install_2ArduinoIDE.html#autotoc_md184", null ]
    ] ],
    [ "CLion", "md_docs_2Install_2CLion.html", [
      [ "追加", "md_docs_2Install_2CLion.html#autotoc_md186", null ],
      [ "更新", "md_docs_2Install_2CLion.html#autotoc_md187", null ],
      [ "submodule 使用時の注意点", "md_docs_2Install_2CLion.html#autotoc_md188", null ]
    ] ],
    [ "Platform IO", "md_docs_2Install_2PlatformIO.html", null ],
    [ "Raspberry Pi Pico セットアップ", "md_docs_2Install_2RaspberryPiPico.html", null ],
    [ "バニラ C++ 環境", "md_docs_2Install_2VanillaCpp.html", null ],
    [ "Visual Studio", "md_docs_2Install_2VisualStudio.html", [
      [ "追加", "md_docs_2Install_2VisualStudio.html#autotoc_md197", null ],
      [ "更新", "md_docs_2Install_2VisualStudio.html#autotoc_md198", null ],
      [ "submodule 使用時の注意点", "md_docs_2Install_2VisualStudio.html#autotoc_md199", null ]
    ] ],
    [ "シリアライザ", "md_docs_2Other_2Serialization.html", [
      [ "個別インクルード", "md_docs_2Other_2Serialization.html#autotoc_md201", null ],
      [ "シリアライズ可能な型", "md_docs_2Other_2Serialization.html#autotoc_md202", null ],
      [ "シリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md203", null ],
      [ "デシリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md204", null ],
      [ "既に定義されている型のシリアライズ、デシリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md205", null ],
      [ "詳細", "md_docs_2Other_2Serialization.html#autotoc_md206", null ],
      [ "API", "md_docs_2Other_2Serialization.html#autotoc_md207", [
        [ "Udon::Serialize(...)", "md_docs_2Other_2Serialization.html#autotoc_md208", null ],
        [ "Udon::Deserialize<T>(...)", "md_docs_2Other_2Serialization.html#autotoc_md209", null ],
        [ "Udon::CanDeserialize(...)", "md_docs_2Other_2Serialization.html#autotoc_md210", null ],
        [ "Udon::SerializedSize<T>()", "md_docs_2Other_2Serialization.html#autotoc_md211", null ]
      ] ],
      [ "サンプル", "md_docs_2Other_2Serialization.html#autotoc_md212", [
        [ "整数型シリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md213", null ],
        [ "ヒープ領域を使用しないシリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md214", null ],
        [ "列挙型シリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md215", null ],
        [ "ユーザー定義型シリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md216", null ],
        [ "メンバに配列を持つユーザー定義型", "md_docs_2Other_2Serialization.html#autotoc_md217", null ],
        [ "ユーザー定義型のネスト", "md_docs_2Other_2Serialization.html#autotoc_md218", null ],
        [ "デシリアライズ失敗", "md_docs_2Other_2Serialization.html#autotoc_md219", null ]
      ] ]
    ] ],
    [ "トレイト", "md_docs_2Other_2Traits.html", [
      [ "メタ関数", "md_docs_2Other_2Traits.html#autotoc_md221", [
        [ "送信クラス要件", "md_docs_2Other_2Traits.html#autotoc_md222", null ],
        [ "受信クラス要件", "md_docs_2Other_2Traits.html#autotoc_md223", null ],
        [ "オーバーロード解決の優先順位制御", "md_docs_2Other_2Traits.html#autotoc_md224", null ],
        [ "AlwaysFalse<T>", "md_docs_2Other_2Traits.html#autotoc_md225", null ]
      ] ],
      [ "曖昧なメンバ関数呼び出し", "md_docs_2Other_2Traits.html#autotoc_md226", [
        [ "基本", "md_docs_2Other_2Traits.html#autotoc_md227", null ],
        [ "実用例", "md_docs_2Other_2Traits.html#autotoc_md228", null ]
      ] ]
    ] ],
    [ "ユーティリティ", "md_docs_2Other_2Utility.html", [
      [ "Show 関数", "md_docs_2Other_2Utility.html#autotoc_md230", [
        [ "ユーザー定義型の出力", "md_docs_2Other_2Utility.html#autotoc_md231", null ]
      ] ],
      [ "Assert 関数", "md_docs_2Other_2Utility.html#autotoc_md232", null ],
      [ "Normalized 関数", "md_docs_2Other_2Utility.html#autotoc_md233", null ]
    ] ],
    [ "スタート", "md_docs_2Start_2Include.html", [
      [ "インクルード", "md_docs_2Start_2Include.html#autotoc_md235", null ],
      [ "本ライブラリについて", "md_docs_2Start_2Include.html#autotoc_md236", null ]
    ] ],
    [ "OpenSiv3D との連携", "md_docs_2Start_2OpenSiv3D.html", [
      [ "連携機能", "md_docs_2Start_2OpenSiv3D.html#autotoc_md238", null ]
    ] ],
    [ "ArrayView", "md_docs_2Types_2ArrayView.html", [
      [ "構築", "md_docs_2Types_2ArrayView.html#autotoc_md240", null ],
      [ "ビューから新しいビューを生成する", "md_docs_2Types_2ArrayView.html#autotoc_md241", null ],
      [ "イテレーション", "md_docs_2Types_2ArrayView.html#autotoc_md242", null ]
    ] ],
    [ "色空間", "md_docs_2Types_2Color.html", [
      [ "RGB 色空間", "md_docs_2Types_2Color.html#autotoc_md244", [
        [ "構築", "md_docs_2Types_2Color.html#autotoc_md245", null ],
        [ "カラーコードへの変換", "md_docs_2Types_2Color.html#autotoc_md246", null ],
        [ "他の色空間へ変換", "md_docs_2Types_2Color.html#autotoc_md247", null ]
      ] ],
      [ "HSV 色空間", "md_docs_2Types_2Color.html#autotoc_md248", [
        [ "構築", "md_docs_2Types_2Color.html#autotoc_md249", null ],
        [ "他の色空間へ変換", "md_docs_2Types_2Color.html#autotoc_md250", null ],
        [ "24bit 値への変換", "md_docs_2Types_2Color.html#autotoc_md251", null ]
      ] ],
      [ "その他関数", "md_docs_2Types_2Color.html#autotoc_md252", [
        [ "Rainbow()", "md_docs_2Types_2Color.html#autotoc_md253", null ]
      ] ]
    ] ],
    [ "オイラー角", "md_docs_2Types_2Eular.html", null ],
    [ "固定ビット長浮動小数点型", "md_docs_2Types_2Float.html", [
      [ "概要", "md_docs_2Types_2Float.html#autotoc_md256", null ],
      [ "構築", "md_docs_2Types_2Float.html#autotoc_md257", null ]
    ] ],
    [ "Optional 型", "md_docs_2Types_2Optional.html", [
      [ "個別インクルード", "md_docs_2Types_2Optional.html#autotoc_md259", null ],
      [ "有/無効値の判定", "md_docs_2Types_2Optional.html#autotoc_md260", null ],
      [ "値の参照", "md_docs_2Types_2Optional.html#autotoc_md261", null ],
      [ "有効値のセット", "md_docs_2Types_2Optional.html#autotoc_md262", null ],
      [ "無効値のセット", "md_docs_2Types_2Optional.html#autotoc_md263", null ],
      [ "本ライブラリでの使われ方", "md_docs_2Types_2Optional.html#autotoc_md264", null ]
    ] ],
    [ "クオータニオン", "md_docs_2Types_2Quaternion.html", null ],
    [ "RingBuffer", "md_docs_2Types_2RingBuffer.html", [
      [ "構築", "md_docs_2Types_2RingBuffer.html#autotoc_md267", null ],
      [ "構築", "md_docs_2Types_2RingBuffer.html#autotoc_md268", null ],
      [ "要素の追加、削除", "md_docs_2Types_2RingBuffer.html#autotoc_md269", null ],
      [ "イテレーション", "md_docs_2Types_2RingBuffer.html#autotoc_md270", null ]
    ] ],
    [ "StaticVector", "md_docs_2Types_2StaticVector.html", [
      [ "構築", "md_docs_2Types_2StaticVector.html#autotoc_md272", null ],
      [ "要素の追加、削除", "md_docs_2Types_2StaticVector.html#autotoc_md273", null ],
      [ "イテレーション", "md_docs_2Types_2StaticVector.html#autotoc_md274", null ]
    ] ],
    [ "StringView", "md_docs_2Types_2StringView.html", [
      [ "個別インクルード", "md_docs_2Types_2StringView.html#autotoc_md276", null ],
      [ "std::string との違い", "md_docs_2Types_2StringView.html#autotoc_md277", null ],
      [ "標準出力への出力", "md_docs_2Types_2StringView.html#autotoc_md278", null ],
      [ "サイズ", "md_docs_2Types_2StringView.html#autotoc_md279", null ],
      [ "文字アクセス", "md_docs_2Types_2StringView.html#autotoc_md280", null ],
      [ "部分文字列作成", "md_docs_2Types_2StringView.html#autotoc_md281", [
        [ "■ 指定された終端文字までの部分文字列を作成", "md_docs_2Types_2StringView.html#autotoc_md282", null ],
        [ "■ 先頭の N 文字を削除した部分文字列を作成する", "md_docs_2Types_2StringView.html#autotoc_md283", null ],
        [ "■ 末尾の N 文字を削除した部分文字列を作成する", "md_docs_2Types_2StringView.html#autotoc_md284", null ],
        [ "■ 指定された区切り文字で区切り部分文字列のリストを作成する", "md_docs_2Types_2StringView.html#autotoc_md285", null ]
      ] ],
      [ "文字列比較", "md_docs_2Types_2StringView.html#autotoc_md286", [
        [ "■ 単純な文字列比較", "md_docs_2Types_2StringView.html#autotoc_md287", null ],
        [ "■ 指定の文字列で始まっているか判定", "md_docs_2Types_2StringView.html#autotoc_md288", null ],
        [ "■ 指定の文字列で終わっているか判定", "md_docs_2Types_2StringView.html#autotoc_md289", null ]
      ] ],
      [ "イテレータ", "md_docs_2Types_2StringView.html#autotoc_md290", null ],
      [ "数値への変換", "md_docs_2Types_2StringView.html#autotoc_md291", null ],
      [ "入れ替え", "md_docs_2Types_2StringView.html#autotoc_md292", null ],
      [ "文字列リテラル", "md_docs_2Types_2StringView.html#autotoc_md293", null ],
      [ "std::string への変換", "md_docs_2Types_2StringView.html#autotoc_md294", null ]
    ] ],
    [ "二次元ベクトル", "md_docs_2Types_2Vector2D.html", [
      [ "構造", "md_docs_2Types_2Vector2D.html#autotoc_md296", null ],
      [ "構築", "md_docs_2Types_2Vector2D.html#autotoc_md297", null ],
      [ "要素へのアクセス", "md_docs_2Types_2Vector2D.html#autotoc_md298", null ],
      [ "ベクトル演算", "md_docs_2Types_2Vector2D.html#autotoc_md299", null ],
      [ "シリアルモニターに送信する", "md_docs_2Types_2Vector2D.html#autotoc_md300", [
        [ "show()", "md_docs_2Types_2Vector2D.html#autotoc_md301", null ]
      ] ],
      [ "ベクトルの長さを求める", "md_docs_2Types_2Vector2D.html#autotoc_md302", [
        [ "length() -> double", "md_docs_2Types_2Vector2D.html#autotoc_md303", null ],
        [ "distanceFrom(Vec2 other) -> double", "md_docs_2Types_2Vector2D.html#autotoc_md304", null ]
      ] ],
      [ "ベクトルの角度を求める", "md_docs_2Types_2Vector2D.html#autotoc_md305", [
        [ "angleAt(Vec2 other) -> double", "md_docs_2Types_2Vector2D.html#autotoc_md306", null ],
        [ "angle() -> double", "md_docs_2Types_2Vector2D.html#autotoc_md307", null ]
      ] ],
      [ "ベクトルの長さを変更する", "md_docs_2Types_2Vector2D.html#autotoc_md308", [
        [ "scaledLength(double length) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md309", null ],
        [ "scaleLength(double length) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md310", null ]
      ] ],
      [ "原点を中心にベクトルを指定角度回転させる", "md_docs_2Types_2Vector2D.html#autotoc_md311", [
        [ "rotated(double angle) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md312", null ],
        [ "rotate(double angle) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md313", null ]
      ] ],
      [ "指定した点を中心にベクトルを指定角度回転させる", "md_docs_2Types_2Vector2D.html#autotoc_md314", [
        [ "rotatedAt(Vec2 center, double angle) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md315", null ],
        [ "rotateAt(Vec2 center, double angle) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md316", null ]
      ] ],
      [ "極座標系へ変換する", "md_docs_2Types_2Vector2D.html#autotoc_md317", [
        [ "toPolar() -> Polar", "md_docs_2Types_2Vector2D.html#autotoc_md318", null ]
      ] ],
      [ "ベクトルの各要素の値を一定の範囲に収める", "md_docs_2Types_2Vector2D.html#autotoc_md319", [
        [ "clamped(double min, double max) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md320", null ],
        [ "clamp(double min, double max) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md321", null ]
      ] ],
      [ "ベクトルをリマッピングする", "md_docs_2Types_2Vector2D.html#autotoc_md322", [
        [ "mapped(double fromMin, double fromMax, double toMin, double toMax) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md323", null ],
        [ "map(double fromMin, double fromMax, double toMin, double toMax) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md324", null ]
      ] ],
      [ "OpenSiv3D との連携", "md_docs_2Types_2Vector2D.html#autotoc_md325", [
        [ "s3d::Vector2D<T> -> Udon::Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md326", null ],
        [ "Udon::Vec2 -> s3d::Vector2D<T>", "md_docs_2Types_2Vector2D.html#autotoc_md327", null ]
      ] ],
      [ "シリアライズ", "md_docs_2Types_2Vector2D.html#autotoc_md328", null ]
    ] ],
    [ "三次元ベクトル", "md_docs_2Types_2Vector3D.html", null ],
    [ "名前空間", "namespaces.html", [
      [ "名前空間一覧", "namespaces.html", "namespaces_dup" ],
      [ "名前空間メンバ", "namespacemembers.html", [
        [ "全て", "namespacemembers.html", null ],
        [ "関数", "namespacemembers_func.html", null ],
        [ "変数", "namespacemembers_vars.html", null ],
        [ "型定義", "namespacemembers_type.html", null ],
        [ "列挙型", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "クラス", "annotated.html", [
      [ "クラス一覧", "annotated.html", "annotated_dup" ],
      [ "クラス索引", "classes.html", null ],
      [ "クラス階層", "hierarchy.html", "hierarchy" ],
      [ "クラスメンバ", "functions.html", [
        [ "全て", "functions.html", "functions_dup" ],
        [ "関数", "functions_func.html", "functions_func" ],
        [ "変数", "functions_vars.html", null ],
        [ "型定義", "functions_type.html", null ],
        [ "関連関数", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "ファイル", "files.html", [
      [ "ファイル一覧", "files.html", "files_dup" ],
      [ "ファイルメンバ", "globals.html", [
        [ "全て", "globals.html", null ],
        [ "マクロ定義", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"AirCylinder_8hpp.html",
"Quaternion_8hpp.html",
"classUdon_1_1BasicStringView.html#a1bdc242a240c0e36897183c52d2b4ee4",
"classUdon_1_1I2cBusImpl.html#a75428bb509303336b67f22d9f35dfe2b",
"classUdon_1_1Impl_1_1Deserializer.html#accfeefa09be65ae2e72b461164cb5fa6",
"classUdon_1_1PadPS5USB.html#a5787122fc897bbefaa9500c1f2149621",
"classUdon_1_1SpeedPidController.html#a34b1b81dc5fd9c4635328ff82d7e3b6f",
"functions_z.html",
"md_docs_2Types_2Color.html#autotoc_md251",
"structUdon_1_1CanTxNode.html#a542aa0315bef4509a4691ac026805ed4",
"structUdon_1_1Message_1_1PadPS5.html#a46f4f60cb2c934ecb9072f09a30523be",
"structUdon_1_1Quaternion.html#aeabf5bf27829661b01fe16194833f685",
"structUdon_1_1Vec2.html#a82b41b3d801e5ece0ac11d0f14b5e61a"
];

var SYNCONMSG = 'クリックで同期表示が無効になります';
var SYNCOFFMSG = 'クリックで同期表示が有効になります';