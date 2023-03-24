#pragma once

#define UDON_INCLUDED

/// @brief 算術関数
#include <udon\math\Math.hpp>

/// @brief フィードバック制御器
#include <udon\algorithm\PidController.hpp>

/// @brief 移動平均
#include <udon\algorithm\MovingAverage.hpp>

/// @brief ループ周期制御
#include <udon\system\LoopCycleController.hpp>

/// @brief ベクトル演算
#include <udon\common\types\Vector2D.hpp>
#include <udon\common\types\Vector3D.hpp>
#include <udon\common\types\Vector4D.hpp>

/// @brief ロボットの位置
#include <udon\common\types\Position.hpp>

/// @brief クオータニオン
#include <udon\common\types\Quaternion.hpp>

/// @brief 曲線
#include <udon\common\types\Spline2D.hpp>
#include <udon\common\types\Spline3D.hpp>

/// @brief 連続する線分
#include <udon\common\types\LineString.hpp>

/// @brief アクチュエーター
#include <udon\actuator\Motor.hpp>

/// @brief シリアライザー
#include <udon\com\serializer\Serializer.hpp>


