//-----------------------------------------------
//
//	UdonLibrary
// 
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------


#pragma once

// ヘッダーを一括でインクルードしない場合、UDON_INCLUDE_SELECTABLE マクロを定義し個別にインクルードしてください。
#ifndef UDON_INCLUDE_SELECTABLE

// UdonLibrary が含まれているかどうかを判定するマクロ
#define UDON_INCLUDED

/////////////////////////////////////////////////////////////////////
//
//  アクチュエーター / actuator
//
/////////////////////////////////////////////////////////////////////

// モーター(基板用)
#include <udon/actuator/MotorDrive.hpp>

// モーター(メイン側)
#include <udon/actuator/Motor.hpp>

// サーボ
#include <udon/actuator/ServoSpeed.hpp>

/////////////////////////////////////////////////////////////////////
//
//  アルゴリズム / algorithm
//
/////////////////////////////////////////////////////////////////////

// 配列参照
#include <udon/algorithm/ArrayView.hpp>

// デリゲート / メンバ関数ポインタ変換
#include <udon/algorithm/Delegate.hpp>

// CRC-8 チェックサム / 誤り検出
#include <udon/algorithm/CRC8.hpp>

// 疑似可変長リングバッファ
#include <udon/algorithm/RingBuffer.hpp>

// PIDフィードバック制御器
#include <udon/algorithm/PidController.hpp>

// 移動平均
#include <udon/algorithm/MovingAverage.hpp>

// ループ周期制御
#include <udon/algorithm/LoopCycleController.hpp>

// 独立ステア最適化
#include <udon/algorithm/SteerOptimizer.hpp>

/////////////////////////////////////////////////////////////////////
//
//  通信 / com
//
/////////////////////////////////////////////////////////////////////

// CAN
#include <udon/com/can/Can.hpp>

// UART
#include <udon/com/uart/Uart.hpp>

// I2c
#include <udon/com/i2c/I2c.hpp>

// im920
#include <udon/com/im920/Im920.hpp>

// フォワーディング
#include <udon/com/common/Forwarding.hpp>

// シリアライズ
#include <udon/com/serialization/Serializer.hpp>

// コントローラー
#include <udon/com/pad/PadPS5.hpp>

// 通信用構造体群
#include <udon/com/message/Message.hpp>

/////////////////////////////////////////////////////////////////////
//
//  ファイルシステム / filesystem
//
/////////////////////////////////////////////////////////////////////

// SDカード
#include <udon/filesystem/sd/Parameter.hpp>

/////////////////////////////////////////////////////////////////////
//
//  数学 / math
//
/////////////////////////////////////////////////////////////////////

// 算術関数
#include <udon/math/Math.hpp>

#include <udon/math/Ceil.hpp>

/////////////////////////////////////////////////////////////////////
//
//  センサー / sensor
//
/////////////////////////////////////////////////////////////////////

// IMU / 慣性計測装置
#include <udon/sensor/Imu.hpp>

// BNO055(IMU)
#include <udon/sensor/BNO055.hpp>

// ディップスイッチ
#include <udon/sensor/DipSwitch.hpp>

/////////////////////////////////////////////////////////////////////
//
//  表示系 / display
//
/////////////////////////////////////////////////////////////////////

// 7セグメントLED
#include <udon/display/SegmentsLed.hpp>

/////////////////////////////////////////////////////////////////////
//
//  ユーザー定義型 / types
//
/////////////////////////////////////////////////////////////////////

// 固定長浮動小数点型
#include <udon/types/Float.hpp>

// 2次元ベクトル
#include <udon/types/Vector2D.hpp>

// 3次元ベクトル
#include <udon/types/Vector3D.hpp>

// 4次元ベクトル
#include <udon/types/Vector4D.hpp>

// オイラー角
#include <udon/types/Euler.hpp>

// 直交座標系
#include <udon/types/Polar.hpp>

// ロボットの位置
#include <udon/types/Position.hpp>

// クオータニオン
#include <udon/types/Quaternion.hpp>

// 連続する線分
#include <udon/types/LineString.hpp>

// 色空間 / RGB / HSV
#include <udon/types/Color.hpp>

/////////////////////////////////////////////////////////////////////
//
//  トレイト / trait
//
/////////////////////////////////////////////////////////////////////

// メンバー関数の有無
#include <udon/traits/HasMember.hpp>

// メンバ関数の呼び出し
#include <udon/traits/MaybeInvoke.hpp>

/////////////////////////////////////////////////////////////////////
//
//  ユーティリティ / utility
//
/////////////////////////////////////////////////////////////////////

// デバッグ / ログ出力
#include <udon/utility/Show.hpp>

#endif  // UDON_INCLUDE_SELECTABLE
