//-------------------------------------------------------------------
//
//    UdonLibrary
// 
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------


#pragma once


// UdonLibrary が含まれているかどうかを判定するマクロ
#define UDON_INCLUDED

//-------------------------------------------------------------------
//
//    プラットフォーム定義
//
//-------------------------------------------------------------------

// プラットフォーム定義
#include "Udon/Common/Platform.hpp"

// 汎用Printf
#include "Udon/Common/Printf.hpp"

// 汎用表示
#include "Udon/Common/Show.hpp"

// 汎用時間取得
#include "Udon/Common/Time.hpp"

//-------------------------------------------------------------------
//
//    アルゴリズム / algorithm
//
//-------------------------------------------------------------------

// デリゲート / メンバ関数ポインタ変換
#include "Udon/Algorithm/Delegate.hpp"

// CRC チェックサム / 誤り検出
#include "Udon/Algorithm/CRC.hpp"

// PIDフィードバック制御器
#include "Udon/Algorithm/PidController.hpp"

// 移動平均
#include "Udon/Algorithm/MovingAverage.hpp"

// ループ周期制御
#include "Udon/Algorithm/LoopCycleController.hpp"

// 独立ステア最適化
#include "Udon/Algorithm/SteerOptimizer.hpp"

// サーボ
#include "Udon/Algorithm/ServoSpeed.hpp"

// 数学
#include "Udon/Algorithm/Math.hpp"


//-------------------------------------------------------------------
//
//    通信 / com
//
//-------------------------------------------------------------------

// CAN通信
#include "Udon/Com/Can.hpp"

// UART通信
#include "Udon/Com/Uart.hpp"

// I2c通信
#include "Udon/Com/I2c.hpp"

// IM920
#include "Udon/Com/Im920.hpp"

// LoRa
#include "Udon/Com/Lora.hpp"

// シリアライズ
#include "Udon/Com/Serialization.hpp"

// 通信用構造体群
#include "Udon/Com/Message.hpp"

// 通信経由モーター
#include "Udon/Com/Driver/Motor.hpp"

// 通信経由エンコーダー
#include "Udon/Com/Driver/Encoder.hpp"

// フォワーディング / メッセージ転送
#include "Udon/Com/Common/Forwarding.hpp"

// 配列受信
#include "Udon/Com/Common/ArrayElementReader.hpp"

// 配列送信
#include "Udon/Com/Common/ArrayElementWriter.hpp"


//-------------------------------------------------------------------
//
//    ファイルシステム / filesystem
//
//-------------------------------------------------------------------

// SDカード
#include "Udon/Filesystem/Sd/Parameter.hpp"


//-------------------------------------------------------------------
//
//    デバイスドライバー
//
//-------------------------------------------------------------------

// モーター(基板用)
#include "Udon/Driver/Motor.hpp"

// BNO055(IMU)
#include "Udon/Driver/BNO055.hpp"

// ディップスイッチ
#include "Udon/Driver/DipSwitch.hpp"

// 7セグメントLED
#include "Udon/Driver/SegmentsLed.hpp"

// LED
#include "Udon/Driver/Led.hpp"

// Raspberry Pi Pico エンコーダー
#include "Udon/Driver/EncoderPico.hpp"

// Raspberry Pi Pico PIO クロック出力
#include "Udon/Driver/Pio/PioClock.hpp"


//-------------------------------------------------------------------
//
//    ユーザー定義型 / types
//
//-------------------------------------------------------------------

// 色空間 / RGB / HSV
#include "Udon/Types/Color.hpp"

// 2次元ベクトル
#include "Udon/Types/Vector2D.hpp"

// 3次元ベクトル
#include "Udon/Types/Vector3D.hpp"

// 4次元ベクトル
#include "Udon/Types/Vector4D.hpp"

// 極座標系
#include "Udon/Types/Polar.hpp"

// ロボットの位置
#include "Udon/Types/Position.hpp"

// 固定長浮動小数点型
#include "Udon/Types/Float.hpp"

// オイラー角
#include "Udon/Types/Euler.hpp"

// クオータニオン
#include "Udon/Types/Quaternion.hpp"

// コンテナ / 配列参照
#include "Udon/Types/ArrayView.hpp"

// コンテナ / 文字列参照 簡易字句解析器
#include "Udon/Types/StringView.hpp"

// コンテナ / 静的可変長配列
#include "Udon/Types/StaticVector.hpp"

// コンテナ / リングバッファ
#include "Udon/Types/RingBuffer.hpp"


//-------------------------------------------------------------------
//
//    トレイト / trait
//
//-------------------------------------------------------------------

// メンバー関数の有無判定
#include "Udon/Traits/HasMemberFunction.hpp"

// コンセプト
#include "Udon/Traits/Concept.hpp"

// 送信クラスであるか
#include "Udon/Traits/IsWriter.hpp"

// 受信クラスであるか
#include "Udon/Traits/IsReader.hpp"

// メンバ変数列挙可能か
#include "Udon/Traits/Accessible.hpp"

// std::void_t
#include "Udon/Traits/VoidT.hpp"

// static_assert 失敗遅延
#include "Udon/Traits/AlwaysFalse.hpp"


//-------------------------------------------------------------------
//
//    ユーティリティ / utility
//
//-------------------------------------------------------------------

// 汎用表示
#include "Udon/Common/Show.hpp"

// フォーマット表示
#include "Udon/Common/Printf.hpp"
