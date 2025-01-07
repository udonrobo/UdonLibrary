//
//    UdonLibrary
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

/// @brief UdonLibrary が含まれているかどうかを判定するマクロ
#define UDON_INCLUDED


//
//    アルゴリズム / algorithm
//

// CRC チェックサム / 誤り検出
#include <Udon/Algorithm/CRC.hpp>

// PIDフィードバック制御器
#include <Udon/Algorithm/PidController.hpp>

// 移動平均
#include <Udon/Algorithm/MovingAverage.hpp>

// ループ周期制御
#include <Udon/Algorithm/LoopCycleController.hpp>

// 独立ステア最適化
#include <Udon/Algorithm/SteerOptimizer.hpp>

// サーボ
#include <Udon/Algorithm/ServoSpeed.hpp>

// 数学
#include <Udon/Algorithm/Math.hpp>


//
//    通信 / com
//

// CAN
#include <Udon/Com/Can.hpp>

// Serial
#include <Udon/Com/Serial.hpp>

// I2c
#include <Udon/Com/I2c.hpp>

// IM920
#include <Udon/Com/Im920.hpp>

// LoRa
#include <Udon/Com/LoRa.hpp>

// コントローラー
#include <Udon/Com/Pad.hpp>

// 通信用構造体群
#include <Udon/Com/Message.hpp>

// 通信経由モーター
#include <Udon/Com/Driver/Motor.hpp>

// 通信経由エンコーダー
#include <Udon/Com/Driver/Encoder.hpp>

// フォワーディング / メッセージ転送
#include <Udon/Com/Common/Forwarding.hpp>


//
//    ファイルシステム / filesystem
//

// SDカード
// #include <Udon/Filesystem/Sd/Parameter.hpp>


//
//    シリアライザー / serializer
//

#include <Udon/Serializer/Serializer.hpp>


//
//    デバイスドライバー / driver
//

// モーター(基板用)
#include <Udon/Driver/Motor.hpp>

// ロボマスモーター
#include <Udon/Driver/RoboMasterMotor.hpp>

// BNO055(IMU)
#include <Udon/Driver/BNO055.hpp>

// ディップスイッチ
#include <Udon/Driver/DipSwitch.hpp>

// 7セグメントLED
#include <Udon/Driver/SegmentsLed.hpp>

// LED
#include <Udon/Driver/Led.hpp>

// Raspberry Pi Pico エンコーダー
#include <Udon/Driver/EncoderPico.hpp>

// Raspberry Pi Pico PIO クロック出力
#include <Udon/Driver/Pio/PioClock.hpp>

// Raspberry Pi Pico サーボ
#include <Udon/Driver/Servo.hpp>


//
//    ユーザー定義型 / types
//

// 色空間 / RGB / HSV
#include <Udon/Types/Color.hpp>

// 2次元ベクトル
#include <Udon/Types/Vector2D.hpp>

// 3次元ベクトル
#include <Udon/Types/Vector3D.hpp>

// 極座標系
#include <Udon/Types/Polar.hpp>

// ロボットの位置
#include <Udon/Types/Position.hpp>

// 固定長浮動小数点型
#include <Udon/Types/Float.hpp>

// オイラー角
#include <Udon/Types/Euler.hpp>

// クオータニオン
#include <Udon/Types/Quaternion.hpp>

// コンテナ / 配列参照
#include <Udon/Types/ArrayView.hpp>

// コンテナ / 文字列参照 簡易字句解析器
#include <Udon/Types/StringView.hpp>

// コンテナ / 静的可変長配列
#include <Udon/Types/StaticVector.hpp>

// コンテナ / リングバッファ
// #include <Udon/Types/RingBuffer.hpp>


//
//    トレイト / trait
//

// メンバー関数の有無判定
#include <Udon/Traits/HasMemberFunction.hpp>

// 送信クラス、受信クラス判定
#include <Udon/Traits/ReaderWriterTraits.hpp>

// シリアライズ可能判定
#include <Udon/Serializer/SerializerTraits.hpp>

// static_assert 失敗遅延
#include <Udon/Traits/AlwaysFalse.hpp>


//
//    ユーティリティ
//

// プラットフォーム定義
#include <Udon/Utility/Platform.hpp>

// 汎用Printf
#include <Udon/Utility/Printf.hpp>

// 汎用表示
#include <Udon/Utility/Show.hpp>

// 汎用時間取得
#include <Udon/Utility/Time.hpp>

// Raspberry Pi Pico 用ウォッチドックタイマー
#include <Udon/Utility/PicoWDT.hpp>

// アサート
#include <Udon/Utility/Assert.hpp>
