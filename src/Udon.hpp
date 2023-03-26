#pragma once

#define UDON_INCLUDED

/// @brief 定義
#include <udon\system\PreDefine.hpp>

/////////////////////////////////////////////////////////////////////
//
//  アクチュエーター
//
/////////////////////////////////////////////////////////////////////

/// @brief アクチュエーター
#include <udon\actuator\Motor.hpp>

/// @brief フィードバック制御器
#include <udon\algorithm\PidController.hpp>

/// @brief 移動平均
#include <udon\algorithm\MovingAverage.hpp>

/// @brief CAN通信
#include <udon\com\can\Can.hpp>

/// @brief コントローラー
#include <udon\com\pad\PadPS5.hpp>

/// @brief 算術関数
#include <udon\math\Math.hpp>

/// @brief ベクトル演算
#include <udon\types\Vector2D.hpp>
#include <udon\types\Vector3D.hpp>
#include <udon\types\Vector4D.hpp>

/// @brief ロボットの位置
#include <udon\types\Position.hpp>

/// @brief クオータニオン
#include <udon\types\Quaternion.hpp>

/// @brief 曲線
#include <udon\types\Spline2D.hpp>
#include <udon\types\Spline3D.hpp>

/// @brief 連続する線分
#include <udon\types\LineString.hpp>

/// @brief シリアライザー
#include <udon\com\serializer\Serializer.hpp>

/// @brief ループ周期制御
#include <udon\system\LoopCycleController.hpp>
