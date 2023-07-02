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
//    アクチュエーター / actuator
//
//-------------------------------------------------------------------

// モーター(基板用)
#include <Udon/Actuator/MotorDriver.hpp>

// モーター(メイン側)
#include <Udon/Actuator/Motor.hpp>

// サーボ
#include <Udon/Actuator/ServoSpeed.hpp>


//-------------------------------------------------------------------
//
//    アルゴリズム / algorithm
//
//-------------------------------------------------------------------


// 配列参照
#include <Udon/Algorithm/ArrayView.hpp>

// デリゲート / メンバ関数ポインタ変換
#include <Udon/Algorithm/Delegate.hpp>

// CRC-8 チェックサム / 誤り検出
#include <Udon/Algorithm/CRC8.hpp>

// 疑似可変長リングバッファ
#include <Udon/Algorithm/RingBuffer.hpp>

// PIDフィードバック制御器
#include <Udon/Algorithm/PidController.hpp>

// 移動平均
#include <Udon/Algorithm/MovingAverage.hpp>

// ループ周期制御
#include <Udon/Algorithm/LoopCycleController.hpp>

// 独立ステア最適化
#include <Udon/Algorithm/SteerOptimizer.hpp>


//-------------------------------------------------------------------
//
//    通信 / com
//
//-------------------------------------------------------------------

// CAN
#include <Udon/Com/Can.hpp>

// UART
#include <Udon/Com/Uart.hpp>

// I2c
#include <Udon/Com/I2c.hpp>

// im920
#include <Udon/Com/Im920.hpp>

// フォワーディング
#include <Udon/Com/Common/Forwarding.hpp>

// 配列送受信
#include <Udon/Com/Common/ArrayReader.hpp>
#include <Udon/Com/Common/ArrayWriter.hpp>

// シリアライズ
 #include <Udon/Com/Serialization.hpp>

// コントローラー
#include <Udon/Com/Pad/PadPS5.hpp>

// 通信用構造体群
#include <Udon/Com/Message.hpp>


//-------------------------------------------------------------------
//
//    ファイルシステム / filesystem
//
//-------------------------------------------------------------------

// SDカード
#include <Udon/Filesystem/Sd/Parameter.hpp>


//-------------------------------------------------------------------
//
//    数学 / math
//
//-------------------------------------------------------------------

#include <Udon/Math/Math.hpp>


//-------------------------------------------------------------------
//
//    センサー / sensor
//
//-------------------------------------------------------------------

// IMU / 慣性計測装置
#include <Udon/Sensor/Imu.hpp>

// BNO055(IMU)
#include <Udon/Sensor/BNO055.hpp>

// ディップスイッチ
#include <Udon/Sensor/DipSwitch.hpp>

// raspberrypi pico エンコーダー
#include <Udon/Sensor/EncoderPico.hpp>


//-------------------------------------------------------------------
//
//    表示系 / display
//
//-------------------------------------------------------------------

// 7セグメントLED
#include <Udon/Display/SegmentsLed.hpp>


//-------------------------------------------------------------------
//
//    ユーザー定義型 / types
//
//-------------------------------------------------------------------

// 色空間
#include <Udon/Types/Color.hpp>

// オイラー角
#include <Udon/Types/Euler.hpp>

// 固定長浮動小数点型
#include <Udon/Types/Float.hpp>

// 直交座標系
#include <Udon/Types/Polar.hpp>

// ロボットの位置
#include <Udon/Types/Position.hpp>

// クオータニオン
#include <Udon/Types/Quaternion.hpp>

// HSV色空間
#include <Udon/Types/HSV.hpp>

// RGB色空間
#include <Udon/Types/RGB.hpp>

// 2次元ベクトル
#include <Udon/Types/Vector2D.hpp>

// 3次元ベクトル
#include <Udon/Types/Vector3D.hpp>

// 4次元ベクトル
#include <Udon/Types/Vector4D.hpp>


//-------------------------------------------------------------------
//
//    型トレイト / trait
//
//-------------------------------------------------------------------

// メンバー関数の有無
#include <Udon/Traits/HasMember.hpp>

// メンバ関数の呼び出し
#include <Udon/Traits/MaybeInvoke.hpp>


//-------------------------------------------------------------------
//
//    ユーティリティ / utility
//
//-------------------------------------------------------------------

// デバッグ / ログ出力
#include <Udon/Utility/Show.hpp>
