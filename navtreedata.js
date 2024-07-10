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
      [ "ユーザー定義型", "index.html#autotoc_md6", null ],
      [ "ドライバー", "index.html#autotoc_md7", null ],
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
      [ "全車輪最適化", "md_docs_2Algorithm_2SteerOptimizer.html#autotoc_md17", null ],
      [ "車輪ごとに最適化", "md_docs_2Algorithm_2SteerOptimizer.html#autotoc_md18", null ],
      [ "Sample", "md_docs_2Algorithm_2SteerOptimizer.html#autotoc_md19", null ]
    ] ],
    [ "CAN 通信", "md_docs_2Communication_2CAN.html", [
      [ "個別インクルード", "md_docs_2Communication_2CAN.html#autotoc_md21", null ],
      [ "通信バスクラス", "md_docs_2Communication_2CAN.html#autotoc_md22", [
        [ "■ Teensy", "md_docs_2Communication_2CAN.html#autotoc_md23", null ],
        [ "■ Raspberry Pi Pico", "md_docs_2Communication_2CAN.html#autotoc_md24", null ]
      ] ],
      [ "送信ノードクラス", "md_docs_2Communication_2CAN.html#autotoc_md25", null ],
      [ "受信ノードクラス", "md_docs_2Communication_2CAN.html#autotoc_md26", null ],
      [ "バイト列を直接送受信", "md_docs_2Communication_2CAN.html#autotoc_md27", [
        [ "■ 送信ノード", "md_docs_2Communication_2CAN.html#autotoc_md28", null ],
        [ "■ 受信ノード", "md_docs_2Communication_2CAN.html#autotoc_md29", null ]
      ] ],
      [ "デバッグ", "md_docs_2Communication_2CAN.html#autotoc_md30", null ],
      [ "エラー原因", "md_docs_2Communication_2CAN.html#autotoc_md31", null ],
      [ "クラスの組み合わせ色々", "md_docs_2Communication_2CAN.html#autotoc_md32", null ]
    ] ],
    [ "通信共通部", "md_docs_2Communication_2Common.html", [
      [ "共通内容", "md_docs_2Communication_2Common.html#autotoc_md34", null ],
      [ "送信者イメージ", "md_docs_2Communication_2Common.html#autotoc_md35", null ],
      [ "受信者イメージ", "md_docs_2Communication_2Common.html#autotoc_md36", null ],
      [ "複数の通信バスや送受信者が存在する場合", "md_docs_2Communication_2Common.html#autotoc_md37", [
        [ "複数送信者", "md_docs_2Communication_2Common.html#autotoc_md38", null ],
        [ "送受信者", "md_docs_2Communication_2Common.html#autotoc_md39", null ],
        [ "複数バス、複数送受信", "md_docs_2Communication_2Common.html#autotoc_md40", null ]
      ] ]
    ] ],
    [ "CRC エラーチェック", "md_docs_2Communication_2CRC.html", [
      [ "チェックサムの概要", "md_docs_2Communication_2CRC.html#autotoc_md42", null ],
      [ "Udon::CRC8", "md_docs_2Communication_2CRC.html#autotoc_md43", null ]
    ] ],
    [ "I2C 通信", "md_docs_2Communication_2I2C.html", [
      [ "個別インクルード", "md_docs_2Communication_2I2C.html#autotoc_md45", null ],
      [ "バスクラス", "md_docs_2Communication_2I2C.html#autotoc_md46", null ],
      [ "マスターからスレーブへ送信", "md_docs_2Communication_2I2C.html#autotoc_md47", [
        [ "マスター送信クラス", "md_docs_2Communication_2I2C.html#autotoc_md48", null ],
        [ "スレーブ受信クラス", "md_docs_2Communication_2I2C.html#autotoc_md49", null ]
      ] ],
      [ "スレーブからマスターへ送信", "md_docs_2Communication_2I2C.html#autotoc_md50", [
        [ "スレーブ送信クラス", "md_docs_2Communication_2I2C.html#autotoc_md51", null ],
        [ "マスター受信クラス", "md_docs_2Communication_2I2C.html#autotoc_md52", null ]
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
      [ "E220", "md_docs_2Communication_2LoRa.html#autotoc_md63", [
        [ "スケッチ例 / 受信側", "md_docs_2Communication_2LoRa.html#autotoc_md64", null ],
        [ "スケッチ例 / 送信側", "md_docs_2Communication_2LoRa.html#autotoc_md65", null ],
        [ "設定値の変更", "md_docs_2Communication_2LoRa.html#autotoc_md66", null ],
        [ "受信強度", "md_docs_2Communication_2LoRa.html#autotoc_md67", null ],
        [ "周波数", "md_docs_2Communication_2LoRa.html#autotoc_md68", null ]
      ] ]
    ] ],
    [ "コントローラ", "md_docs_2Communication_2Pad.html", [
      [ "コントローラーとの通信基板を構成する", "md_docs_2Communication_2Pad.html#autotoc_md70", [
        [ "■ 920MHz 経由 / 無線モジュールがメイン基板にある場合 (推奨)", "md_docs_2Communication_2Pad.html#autotoc_md71", null ],
        [ "■ 920MHz 経由 / 無線モジュールとメイン基板が別々の場合 (推奨)", "md_docs_2Communication_2Pad.html#autotoc_md72", null ],
        [ "■ Bluetooth 経由 / USB ホストシールドがメイン基板にある場合", "md_docs_2Communication_2Pad.html#autotoc_md73", null ],
        [ "■ Bluetooth 経由 / USB ホストシールドとメイン基板が別々の場合", "md_docs_2Communication_2Pad.html#autotoc_md74", null ],
        [ "■ USB 経由", "md_docs_2Communication_2Pad.html#autotoc_md75", null ],
        [ "■ ペアリング方法", "md_docs_2Communication_2Pad.html#autotoc_md76", null ]
      ] ],
      [ "メインマイコン用クラス", "md_docs_2Communication_2Pad.html#autotoc_md77", [
        [ "個別インクルード", "md_docs_2Communication_2Pad.html#autotoc_md78", null ],
        [ "インスタンス化", "md_docs_2Communication_2Pad.html#autotoc_md79", [
          [ "■ I2C 通信経由", "md_docs_2Communication_2Pad.html#autotoc_md80", null ],
          [ "■ CAN 通信経由", "md_docs_2Communication_2Pad.html#autotoc_md81", null ],
          [ "■ メイン基板に無線モジュールがある場合", "md_docs_2Communication_2Pad.html#autotoc_md82", null ],
          [ "■ メイン基板に USB ホストシールドがある場合", "md_docs_2Communication_2Pad.html#autotoc_md83", null ],
          [ "■ USB 通信経由", "md_docs_2Communication_2Pad.html#autotoc_md84", null ],
          [ "■ その他の通信経由", "md_docs_2Communication_2Pad.html#autotoc_md85", null ],
          [ "■ OpenSiv3D 使用時", "md_docs_2Communication_2Pad.html#autotoc_md86", null ]
        ] ],
        [ "更新", "md_docs_2Communication_2Pad.html#autotoc_md87", null ],
        [ "コントローラの状態取得", "md_docs_2Communication_2Pad.html#autotoc_md88", null ],
        [ "ボタン", "md_docs_2Communication_2Pad.html#autotoc_md89", null ],
        [ "スティック", "md_docs_2Communication_2Pad.html#autotoc_md90", null ],
        [ "最終的なスケッチ例", "md_docs_2Communication_2Pad.html#autotoc_md91", null ]
      ] ],
      [ "送信側マイコン用クラス", "md_docs_2Communication_2Pad.html#autotoc_md92", null ],
      [ "受信側マイコン用クラス", "md_docs_2Communication_2Pad.html#autotoc_md93", null ],
      [ "Bluetooth 使用時の受信側マイコン用クラス", "md_docs_2Communication_2Pad.html#autotoc_md94", null ],
      [ "Bluetooth ドングルとのペアリング", "md_docs_2Communication_2Pad.html#autotoc_md95", null ]
    ] ],
    [ "UART", "md_docs_2Communication_2UART.html", null ],
    [ "謝辞", "md_docs_2Developer_2Acknowledgments.html", null ],
    [ "自動テスト", "md_docs_2Developer_2CI.html", [
      [ "Arduino Lint", "md_docs_2Developer_2CI.html#autotoc_md99", [
        [ "■ 概要", "md_docs_2Developer_2CI.html#autotoc_md100", null ],
        [ "■ ディレクトリを追加する", "md_docs_2Developer_2CI.html#autotoc_md101", null ],
        [ "■ テストファイルを追加する", "md_docs_2Developer_2CI.html#autotoc_md102", null ],
        [ "■ コンパイル時に計算可能なアルゴリズムのテスト", "md_docs_2Developer_2CI.html#autotoc_md103", null ],
        [ "■ 検証ボードを追加する", "md_docs_2Developer_2CI.html#autotoc_md104", null ],
        [ "■ ローカル環境で実行", "md_docs_2Developer_2CI.html#autotoc_md105", null ]
      ] ],
      [ "Google Unit Test", "md_docs_2Developer_2CI.html#autotoc_md106", [
        [ "■ 概要", "md_docs_2Developer_2CI.html#autotoc_md107", null ],
        [ "■ ディレクトリを追加する", "md_docs_2Developer_2CI.html#autotoc_md108", null ],
        [ "■ ソースファイルを追加する", "md_docs_2Developer_2CI.html#autotoc_md109", null ],
        [ "■ テストを書く", "md_docs_2Developer_2CI.html#autotoc_md110", null ],
        [ "■ ローカル環境で実行", "md_docs_2Developer_2CI.html#autotoc_md111", null ]
      ] ]
    ] ],
    [ "開発環境セットアップ", "md_docs_2Developer_2DevelopSetup.html", [
      [ "開発環境", "md_docs_2Developer_2DevelopSetup.html#autotoc_md113", null ],
      [ "VSCode", "md_docs_2Developer_2DevelopSetup.html#autotoc_md114", [
        [ "ライブラリを開く", "md_docs_2Developer_2DevelopSetup.html#autotoc_md115", null ],
        [ "clang-format 拡張機能インストール", "md_docs_2Developer_2DevelopSetup.html#autotoc_md116", null ],
        [ "C/C++ 拡張機能インストール", "md_docs_2Developer_2DevelopSetup.html#autotoc_md117", null ],
        [ "スペルチェッカー拡張機能インストール", "md_docs_2Developer_2DevelopSetup.html#autotoc_md118", null ]
      ] ],
      [ "Visual Studio", "md_docs_2Developer_2DevelopSetup.html#autotoc_md119", [
        [ "ライブラリを開く", "md_docs_2Developer_2DevelopSetup.html#autotoc_md120", null ],
        [ "clang-format", "md_docs_2Developer_2DevelopSetup.html#autotoc_md121", null ]
      ] ],
      [ "GitHub Copilot 導入", "md_docs_2Developer_2DevelopSetup.html#autotoc_md122", null ]
    ] ],
    [ "ディレクトリ構造", "md_docs_2Developer_2DirectoryStructure.html", null ],
    [ "ドキュメント自動生成", "md_docs_2Developer_2DoxygenPage.html", [
      [ "ローカル環境で実行", "md_docs_2Developer_2DoxygenPage.html#autotoc_md125", null ]
    ] ],
    [ "■ 静的変数編", "md_docs_2Developer_2Idea.html", [
      [ "■ 静的関数編", "md_docs_2Developer_2Idea.html#autotoc_md129", null ],
      [ "■ 型の相互変換編", "md_docs_2Developer_2Idea.html#autotoc_md130", [
        [ "問題", "md_docs_2Developer_2Idea.html#autotoc_md127", null ],
        [ "解決", "md_docs_2Developer_2Idea.html#autotoc_md128", null ],
        [ "問題", "md_docs_2Developer_2Idea.html#autotoc_md131", null ],
        [ "解決", "md_docs_2Developer_2Idea.html#autotoc_md132", null ]
      ] ]
    ] ],
    [ "スタイルガイド", "md_docs_2Developer_2StyleGuide.html", [
      [ "言語", "md_docs_2Developer_2StyleGuide.html#autotoc_md134", null ],
      [ "書式", "md_docs_2Developer_2StyleGuide.html#autotoc_md135", null ],
      [ "命名規則", "md_docs_2Developer_2StyleGuide.html#autotoc_md136", [
        [ "ディレクトリ(フォルダ)、ファイル", "md_docs_2Developer_2StyleGuide.html#autotoc_md137", null ],
        [ "変数 定数", "md_docs_2Developer_2StyleGuide.html#autotoc_md138", null ],
        [ "関数", "md_docs_2Developer_2StyleGuide.html#autotoc_md139", null ],
        [ "型", "md_docs_2Developer_2StyleGuide.html#autotoc_md140", null ],
        [ "テンプレート", "md_docs_2Developer_2StyleGuide.html#autotoc_md141", null ],
        [ "名前空間", "md_docs_2Developer_2StyleGuide.html#autotoc_md142", null ],
        [ "マクロ", "md_docs_2Developer_2StyleGuide.html#autotoc_md143", null ]
      ] ]
    ] ],
    [ "BNO055", "md_docs_2Driver_2BNO055.html", null ],
    [ "ディップスイッチ", "md_docs_2Driver_2DipSwitch.html", [
      [ "個別インクルード", "md_docs_2Driver_2DipSwitch.html#autotoc_md146", null ],
      [ "値読み取り", "md_docs_2Driver_2DipSwitch.html#autotoc_md147", null ]
    ] ],
    [ "エンコーダー", "md_docs_2Driver_2EncoderPico.html", [
      [ "個別インクルード", "md_docs_2Driver_2EncoderPico.html#autotoc_md149", null ],
      [ "スケッチ例", "md_docs_2Driver_2EncoderPico.html#autotoc_md150", null ]
    ] ],
    [ "モーター", "md_docs_2Driver_2Motor.html", [
      [ "個別インクルード", "md_docs_2Driver_2Motor.html#autotoc_md152", null ],
      [ "Motor2 クラス", "md_docs_2Driver_2Motor.html#autotoc_md153", null ],
      [ "Motor3 クラス", "md_docs_2Driver_2Motor.html#autotoc_md154", null ],
      [ "デバッグ", "md_docs_2Driver_2Motor.html#autotoc_md155", null ],
      [ "その他", "md_docs_2Driver_2Motor.html#autotoc_md156", null ]
    ] ],
    [ "PIO", "md_docs_2Driver_2PIO.html", [
      [ "PioClockBegin", "md_docs_2Driver_2PIO.html#autotoc_md158", null ],
      [ "AllocateStateMachine", "md_docs_2Driver_2PIO.html#autotoc_md159", null ]
    ] ],
    [ "ロボマスモーター", "md_docs_2Driver_2RoboMasterMotor.html", [
      [ "個別インクルード", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md161", null ],
      [ "概要", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md162", null ],
      [ "電流値の指定", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md163", null ],
      [ "センサー値取得", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md164", null ],
      [ "複数モーター", "md_docs_2Driver_2RoboMasterMotor.html#autotoc_md165", null ]
    ] ],
    [ "Arduino IDE", "md_docs_2Install_2ArduinoIDE.html", [
      [ "追加", "md_docs_2Install_2ArduinoIDE.html#autotoc_md167", null ],
      [ "更新", "md_docs_2Install_2ArduinoIDE.html#autotoc_md168", null ]
    ] ],
    [ "CLion", "md_docs_2Install_2CLion.html", [
      [ "追加", "md_docs_2Install_2CLion.html#autotoc_md170", null ],
      [ "更新", "md_docs_2Install_2CLion.html#autotoc_md171", null ],
      [ "submodule 使用時の注意点", "md_docs_2Install_2CLion.html#autotoc_md172", null ]
    ] ],
    [ "Platform IO", "md_docs_2Install_2PlatformIO.html", null ],
    [ "Raspberry Pi Pico セットアップ", "md_docs_2Install_2RaspberryPiPico.html", null ],
    [ "必要ソフトウェアのインストール", "md_docs_2Install_2RequiredTools.html", [
      [ "ターミナル (端末)", "md_docs_2Install_2RequiredTools.html#autotoc_md176", [
        [ "Windows", "md_docs_2Install_2RequiredTools.html#autotoc_md177", null ]
      ] ],
      [ "git", "md_docs_2Install_2RequiredTools.html#autotoc_md178", [
        [ "Windows", "md_docs_2Install_2RequiredTools.html#autotoc_md179", null ],
        [ "Linux", "md_docs_2Install_2RequiredTools.html#autotoc_md180", null ],
        [ "Mac OS", "md_docs_2Install_2RequiredTools.html#autotoc_md181", null ]
      ] ]
    ] ],
    [ "Visual Studio", "md_docs_2Install_2VisualStudio.html", [
      [ "追加", "md_docs_2Install_2VisualStudio.html#autotoc_md183", null ],
      [ "更新", "md_docs_2Install_2VisualStudio.html#autotoc_md184", null ],
      [ "submodule 使用時の注意点", "md_docs_2Install_2VisualStudio.html#autotoc_md185", null ]
    ] ],
    [ "汎用的な関数", "md_docs_2Other_2Functions.html", [
      [ "Assert()", "md_docs_2Other_2Functions.html#autotoc_md187", null ],
      [ "Normalized()", "md_docs_2Other_2Functions.html#autotoc_md188", null ]
    ] ],
    [ "シリアライザ", "md_docs_2Other_2Serialization.html", [
      [ "個別インクルード", "md_docs_2Other_2Serialization.html#autotoc_md190", null ],
      [ "シリアライズ可能な型", "md_docs_2Other_2Serialization.html#autotoc_md191", null ],
      [ "シリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md192", null ],
      [ "デシリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md193", null ],
      [ "既に定義されている型のシリアライズ、デシリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md194", null ],
      [ "詳細", "md_docs_2Other_2Serialization.html#autotoc_md195", null ],
      [ "API", "md_docs_2Other_2Serialization.html#autotoc_md196", [
        [ "Udon::Serialize(...)", "md_docs_2Other_2Serialization.html#autotoc_md197", null ],
        [ "Udon::Deserialize<T>(...)", "md_docs_2Other_2Serialization.html#autotoc_md198", null ],
        [ "Udon::CanDeserialize(...)", "md_docs_2Other_2Serialization.html#autotoc_md199", null ],
        [ "Udon::SerializedSize<T>()", "md_docs_2Other_2Serialization.html#autotoc_md200", null ]
      ] ],
      [ "サンプル", "md_docs_2Other_2Serialization.html#autotoc_md201", [
        [ "整数型シリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md202", null ],
        [ "ヒープ領域を使用しないシリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md203", null ],
        [ "列挙型シリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md204", null ],
        [ "ユーザー定義型シリアライズ", "md_docs_2Other_2Serialization.html#autotoc_md205", null ],
        [ "メンバに配列を持つユーザー定義型", "md_docs_2Other_2Serialization.html#autotoc_md206", null ],
        [ "ユーザー定義型のネスト", "md_docs_2Other_2Serialization.html#autotoc_md207", null ],
        [ "デシリアライズ失敗", "md_docs_2Other_2Serialization.html#autotoc_md208", null ]
      ] ]
    ] ],
    [ "トレイト", "md_docs_2Other_2Traits.html", [
      [ "メタ関数", "md_docs_2Other_2Traits.html#autotoc_md210", [
        [ "送信クラス要件", "md_docs_2Other_2Traits.html#autotoc_md211", null ],
        [ "受信クラス要件", "md_docs_2Other_2Traits.html#autotoc_md212", null ],
        [ "オーバーロード解決の優先順位制御", "md_docs_2Other_2Traits.html#autotoc_md213", null ],
        [ "AlwaysFalse<T>", "md_docs_2Other_2Traits.html#autotoc_md214", null ]
      ] ],
      [ "曖昧なメンバ関数呼び出し", "md_docs_2Other_2Traits.html#autotoc_md215", [
        [ "基本", "md_docs_2Other_2Traits.html#autotoc_md216", null ],
        [ "実用例", "md_docs_2Other_2Traits.html#autotoc_md217", null ]
      ] ]
    ] ],
    [ "スタート", "md_docs_2Start_2Include.html", [
      [ "インクルード", "md_docs_2Start_2Include.html#autotoc_md219", null ],
      [ "本ライブラリについて", "md_docs_2Start_2Include.html#autotoc_md220", null ]
    ] ],
    [ "OpenSiv3D との連携", "md_docs_2Start_2OpenSiv3D.html", [
      [ "連携機能", "md_docs_2Start_2OpenSiv3D.html#autotoc_md222", null ]
    ] ],
    [ "ArrayView", "md_docs_2Types_2ArrayView.html", [
      [ "構築", "md_docs_2Types_2ArrayView.html#autotoc_md224", null ],
      [ "ビューから新しいビューを生成する", "md_docs_2Types_2ArrayView.html#autotoc_md225", null ],
      [ "イテレーション", "md_docs_2Types_2ArrayView.html#autotoc_md226", null ]
    ] ],
    [ "色空間", "md_docs_2Types_2Color.html", [
      [ "RGB 色空間", "md_docs_2Types_2Color.html#autotoc_md228", [
        [ "構築", "md_docs_2Types_2Color.html#autotoc_md229", null ],
        [ "カラーコードへの変換", "md_docs_2Types_2Color.html#autotoc_md230", null ],
        [ "他の色空間へ変換", "md_docs_2Types_2Color.html#autotoc_md231", null ]
      ] ],
      [ "HSV 色空間", "md_docs_2Types_2Color.html#autotoc_md232", [
        [ "概要", "md_docs_2Types_2Color.html#autotoc_md233", null ],
        [ "構築", "md_docs_2Types_2Color.html#autotoc_md234", null ],
        [ "他の色空間へ変換", "md_docs_2Types_2Color.html#autotoc_md235", null ],
        [ "24bit 値への変換", "md_docs_2Types_2Color.html#autotoc_md236", null ]
      ] ],
      [ "その他関数", "md_docs_2Types_2Color.html#autotoc_md237", [
        [ "Rainbow()", "md_docs_2Types_2Color.html#autotoc_md238", null ]
      ] ]
    ] ],
    [ "オイラー角", "md_docs_2Types_2Eular.html", null ],
    [ "固定ビット長浮動小数点型", "md_docs_2Types_2Float.html", [
      [ "概要", "md_docs_2Types_2Float.html#autotoc_md241", null ],
      [ "構築", "md_docs_2Types_2Float.html#autotoc_md242", null ]
    ] ],
    [ "クオータニオン", "md_docs_2Types_2Quaternion.html", null ],
    [ "RingBuffer", "md_docs_2Types_2RingBuffer.html", [
      [ "構築", "md_docs_2Types_2RingBuffer.html#autotoc_md245", null ],
      [ "構築", "md_docs_2Types_2RingBuffer.html#autotoc_md246", null ],
      [ "要素の追加、削除", "md_docs_2Types_2RingBuffer.html#autotoc_md247", null ],
      [ "イテレーション", "md_docs_2Types_2RingBuffer.html#autotoc_md248", null ]
    ] ],
    [ "StaticVector", "md_docs_2Types_2StaticVector.html", [
      [ "構築", "md_docs_2Types_2StaticVector.html#autotoc_md250", null ],
      [ "要素の追加、削除", "md_docs_2Types_2StaticVector.html#autotoc_md251", null ],
      [ "イテレーション", "md_docs_2Types_2StaticVector.html#autotoc_md252", null ]
    ] ],
    [ "StringView", "md_docs_2Types_2StringView.html", [
      [ "個別インクルード", "md_docs_2Types_2StringView.html#autotoc_md254", null ],
      [ "概要", "md_docs_2Types_2StringView.html#autotoc_md255", null ]
    ] ],
    [ "二次元ベクトル", "md_docs_2Types_2Vector2D.html", [
      [ "構造", "md_docs_2Types_2Vector2D.html#autotoc_md257", null ],
      [ "構築", "md_docs_2Types_2Vector2D.html#autotoc_md258", null ],
      [ "要素へのアクセス", "md_docs_2Types_2Vector2D.html#autotoc_md259", null ],
      [ "ベクトル演算", "md_docs_2Types_2Vector2D.html#autotoc_md260", null ],
      [ "シリアルモニターに送信する", "md_docs_2Types_2Vector2D.html#autotoc_md261", [
        [ "show()", "md_docs_2Types_2Vector2D.html#autotoc_md262", null ]
      ] ],
      [ "ベクトルの長さを求める", "md_docs_2Types_2Vector2D.html#autotoc_md263", [
        [ "length() -> double", "md_docs_2Types_2Vector2D.html#autotoc_md264", null ],
        [ "distanceFrom(Vec2 other) -> double", "md_docs_2Types_2Vector2D.html#autotoc_md265", null ]
      ] ],
      [ "ベクトルの角度を求める", "md_docs_2Types_2Vector2D.html#autotoc_md266", [
        [ "angleAt(Vec2 other) -> double", "md_docs_2Types_2Vector2D.html#autotoc_md267", null ],
        [ "angle() -> double", "md_docs_2Types_2Vector2D.html#autotoc_md268", null ]
      ] ],
      [ "ベクトルの長さを変更する", "md_docs_2Types_2Vector2D.html#autotoc_md269", [
        [ "scaledLength(double length) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md270", null ],
        [ "scaleLength(double length) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md271", null ]
      ] ],
      [ "原点を中心にベクトルを指定角度回転させる", "md_docs_2Types_2Vector2D.html#autotoc_md272", [
        [ "rotated(double angle) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md273", null ],
        [ "rotate(double angle) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md274", null ]
      ] ],
      [ "指定した点を中心にベクトルを指定角度回転させる", "md_docs_2Types_2Vector2D.html#autotoc_md275", [
        [ "rotatedAt(Vec2 center, double angle) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md276", null ],
        [ "rotateAt(Vec2 center, double angle) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md277", null ]
      ] ],
      [ "極座標系へ変換する", "md_docs_2Types_2Vector2D.html#autotoc_md278", [
        [ "toPolar() -> Polar", "md_docs_2Types_2Vector2D.html#autotoc_md279", null ]
      ] ],
      [ "ベクトルの各要素の値を一定の範囲に収める", "md_docs_2Types_2Vector2D.html#autotoc_md280", [
        [ "clamped(double min, double max) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md281", null ],
        [ "clamp(double min, double max) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md282", null ]
      ] ],
      [ "ベクトルをリマッピングする", "md_docs_2Types_2Vector2D.html#autotoc_md283", [
        [ "mapped(double fromMin, double fromMax, double toMin, double toMax) -> Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md284", null ],
        [ "map(double fromMin, double fromMax, double toMin, double toMax) -> Vec2&", "md_docs_2Types_2Vector2D.html#autotoc_md285", null ]
      ] ],
      [ "OpenSiv3D との連携", "md_docs_2Types_2Vector2D.html#autotoc_md286", [
        [ "s3d::Vector2D<T> -> Udon::Vec2", "md_docs_2Types_2Vector2D.html#autotoc_md287", null ],
        [ "Udon::Vec2 -> s3d::Vector2D<T>", "md_docs_2Types_2Vector2D.html#autotoc_md288", null ]
      ] ],
      [ "シリアライズ", "md_docs_2Types_2Vector2D.html#autotoc_md289", null ]
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
"classUdon_1_1BasicStringView.html#a1e35b6e479f90c2b31637e3c77188f38",
"classUdon_1_1I2cBusImpl.html#aaa3199242d834daf7a6fd16ae4d81d10",
"classUdon_1_1Impl_1_1PadPS5OnboardUSBReader.html#a605321434c5c407073d44b90e635093c",
"classUdon_1_1PidController.html#a60133038754ac22f497961e3598be136",
"classUdon_1_1StaticVector.html#ac19b13c22b41afc1d217a7f99b431e0b",
"md_docs_2Communication_2Pad.html#autotoc_md81",
"namespaceUdon.html#aab18b58d62367941f2e34d1d1fde4adb",
"structUdon_1_1Im920Node.html#ab4022e30c4aa6059c9d428dc3443529b",
"structUdon_1_1Polar.html#a94fe23336cc8f7a4ce4bb53dd71448d8",
"structUdon_1_1RingBuffer_1_1iterator.html#a8e34008946f21f8a91a4e01c59edda91"
];

var SYNCONMSG = 'クリックで同期表示が無効になります';
var SYNCOFFMSG = 'クリックで同期表示が有効になります';