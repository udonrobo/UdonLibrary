///   @file  Message.hpp
///   @brief 通信用構造体群
///   @date  2022/12/14

#pragma once

/// @brief 構造体メモリアライン除去マクロ
#ifdef __GNUC__ /// gcc
#define PACK(Ty) Ty __attribute__((__packed__))
#endif

#ifdef _MSC_VER /// msvc
#define PACK(Ty) __pragma(pack(push, 1)) Ty __pragma(pack(pop))
#endif

/// @brief アーキテクチャの差埋め
/// 小数型 : 4bytes
#ifdef _MSC_VER
using floatPoint_t = float;
#elif __SIZEOF_DOUBLE__ == 8 && __SIZEOF_FLOAT__ == 4
using floatPoint_t = float;
#elif __SIZEOF_DOUBLE__ == 4 && __SIZEOF_FLOAT__ == 4
using floatPoint_t = double;
#else
#error No appropriate type found
#endif

/// @brief 座標系
namespace Message
{

	/// @brief 2次元ベクトル
	/// @tparam Ty
	PACK(template <class Ty> struct Vector2D {
		Ty x;
		Ty y;
	});

	/// @brief 3次元ベクトル
	/// @tparam Ty
	PACK(template <class Ty> struct Vector3D {
		Ty x;
		Ty y;
		Ty z;
	});

	/// @brief ロボット座標系
	/// @tparam Ty
	PACK(template <class Ty> struct Position {
		Ty x;
		Ty y;
		Ty yaw;
	});

	/// @brief 2次元ベクトル小数型
	using Vec2 = Vector2D<floatPoint_t>;

	/// @brief 3次元ベクトル小数型
	using Vec3 = Vector3D<floatPoint_t>;

	/// @brief ロボット座標系小数型
	using Pos = Position<floatPoint_t>;

}

/// @brief センサー系
namespace Message
{

	/// @brief エンコーダー
	PACK(struct Encoder {
		int32_t count;
	});

	/// @brief スイッチ
	PACK(struct Switch {
		bool isPress;
	});
	PACK(template <size_t N> struct Switches {
		Switch sw[N];
	});

	/// @brief ジャイロ
	PACK(struct Gyro {
		floatPoint_t roll;
		floatPoint_t pitch;
		floatPoint_t yaw;
	});
	PACK(struct GyroYaw {
		floatPoint_t yaw;
	});

	/// @brief 緊急非常停止
	using EmergencySwitch = Switch;

}

/// @brief アクチュエーター系
namespace Message
{

	PACK(struct Steering {
		int16_t power;
		floatPoint_t angle;
	});

	/// @brief モーター
	PACK(struct Motor {
		int16_t power;
	});

	/// @brief サーボモーター
	PACK(struct Servo {
		uint16_t angle;
	});

	/// @brief ブラシレス
	PACK(struct BrushLess {
		int32_t speed;
	});

	/// @brief ソレノイド
	PACK(struct Solenoid {
		bool release;
	});

	/// @brief ステッピングモーター
	PACK(struct Stepping {
		int32_t count;
	});

	/// @brief ダイナミクセル
	using Dynamixel = Servo;

	/// @brief エアシリンダー
	using AirCylinder = Solenoid;

}

/// @brief 文字列
namespace Message
{

	PACK(template <size_t N> struct String {
		char str[N];
	});

}

/// @brief 表示系
namespace Message
{

	PACK(struct Lcd1602 {
		char top[16];
		char bottom[16];
	});

	PACK(struct Lcd2004 {
		char line1[20];
		char line2[20];
		char line3[20];
		char line4[20];
	});

}

/// @brief 色
namespace Message
{

	/// @brief RGB
	PACK(struct RGB {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	});

	/// @brief HSV
	PACK(struct HSV {
		uint8_t hue;
		uint8_t saturation;
		uint8_t value;
	});

	/// @brief HSL
	PACK(struct HSL {
		uint8_t hue;
		uint8_t saturation;
		uint8_t lightness;
	});

}

/// @brief コントローラー
namespace Message
{
	PACK(struct ControllerPS4 {
		bool share : 1;
		bool l3 : 1;
		bool r3 : 1;
		bool start : 1;
		bool up : 1;
		bool right : 1;
		bool down : 1;
		bool left : 1;

		bool l2 : 1;
		bool r2 : 1;
		bool l1 : 1;
		bool r1 : 1;
		bool triangle : 1;
		bool circle : 1;
		bool cross : 1;
		bool square : 1;

		uint8_t rightHatX;
		uint8_t rightHatY;
		uint8_t leftHatX;
		uint8_t leftHatY;
		uint8_t l2Analog;
		uint8_t r2Analog;

		bool isConnected : 1;
		bool touch : 1;
	});

	PACK(struct ControllerPS5{});

}
