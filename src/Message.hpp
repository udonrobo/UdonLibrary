///   @file  Message.hpp
///   @brief 通信用構造体群
///   @date  2022/12/14

#pragma once

#include <Arduino.h>

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
namespace Message {

    /// @brief 2次元ベクトル
    /// @tparam Ty
    PACK(template <class Ty> struct Vector2D {
        Ty x;
        Ty y;
        void show() const {
            Serial.print("x: ");
            Serial.print(x);
            Serial.print('\t');

            Serial.print("y: ");
            Serial.print(y);
            Serial.print('\t');
        }
    });

    /// @brief 3次元ベクトル
    /// @tparam Ty
    PACK(template <class Ty> struct Vector3D {
        Ty x;
        Ty y;
        Ty z;
        void show() const {
            Serial.print("x: ");
            Serial.print(x);
            Serial.print('\t');

            Serial.print("y: ");
            Serial.print(y);
            Serial.print('\t');

            Serial.print("z: ");
            Serial.print(z);
            Serial.print('\t');
        }
    });

    /// @brief ロボット座標系
    /// @tparam Ty
    PACK(template <class Ty> struct Position {
        Ty x;
        Ty y;
        Ty yaw;
        void show() const {
            Serial.print("x: ");
            Serial.print(x);
            Serial.print('\t');

            Serial.print("y: ");
            Serial.print(y);
            Serial.print('\t');

            Serial.print("yaw: ");
            Serial.print(yaw);
            Serial.print('\t');
        }
    });

    /// @brief 2次元ベクトル小数型
    using Vec2 = Vector2D<floatPoint_t>;

    /// @brief 3次元ベクトル小数型
    using Vec3 = Vector3D<floatPoint_t>;

    /// @brief ロボット座標系小数型
    using Pos = Position<floatPoint_t>;

} // namespace Message

/// @brief センサー系
namespace Message {

    /// @brief エンコーダー
    PACK(struct Encoder {
        int32_t count;
        void show() const {
            Serial.print("count: ");
            Serial.print(count);
            Serial.print('\t');
        }
    });

    /// @brief スイッチ
    PACK(struct Switch {
        bool isPress;
        void show() const {
            Serial.print("press: ");
            Serial.print(isPress);
            Serial.print('\t');
        }
    });
    PACK(template <size_t N> struct Switches {
        Switch sw[N];
        void show() const {
            Serial.print("press: ");
            for (auto &&it : sw) {
                Serial.print(sw.isPress);
            }
            Serial.print('\t');
        }
    });

    /// @brief ジャイロ
    PACK(struct Gyro {
        floatPoint_t roll;
        floatPoint_t pitch;
        floatPoint_t yaw;
        void show() const {
            Serial.print("roll: ");
            Serial.print(roll);
            Serial.print('\t');

            Serial.print("pitch: ");
            Serial.print(pitch);
            Serial.print('\t');

            Serial.print("yaw: ");
            Serial.print(yaw);
            Serial.print('\t');
        }
    });
    PACK(struct GyroYaw {
        floatPoint_t yaw;
        void show() const {
            Serial.print("yaw: ");
            Serial.print(yaw);
            Serial.print('\t');
        }
    });

    /// @brief 緊急非常停止
    using EmergencySwitch = Switch;

} // namespace Message

/// @brief アクチュエーター系
namespace Message {

    PACK(struct Steering {
        int16_t power;
        floatPoint_t angle;
        void show() const {
            Serial.print("power: ");
            Serial.print(power);
            Serial.print('\t');

            Serial.print("angle: ");
            Serial.print(angle);
            Serial.print('\t');
        }
    });

    /// @brief モーター
    PACK(struct Motor {
        int16_t power;
        void show() const {
            Serial.print("power: ");
            Serial.print(power);
            Serial.print('\t');
        }
    });

    /// @brief サーボモーター
    PACK(struct Servo {
        uint16_t angle;
        void show() const {
            Serial.print("angle: ");
            Serial.print(angle);
            Serial.print('\t');
        }
    });

    /// @brief ブラシレス
    PACK(struct BrushLess {
        int32_t speed;
        void show() const {
            Serial.print("speed: ");
            Serial.print(speed);
            Serial.print('\t');
        }
    });

    /// @brief ソレノイド
    PACK(struct Solenoid {
        bool release;
        void show() const {
            Serial.print("release: ");
            Serial.print(release);
            Serial.print('\t');
        }
    });

    /// @brief ステッピングモーター
    PACK(struct Stepping {
        int32_t count;
        void show() const {
            Serial.print("count: ");
            Serial.print(count);
            Serial.print('\t');
        }
    });

    /// @brief ダイナミクセル
    using Dynamixel = Servo;

    /// @brief エアシリンダー
    using AirCylinder = Solenoid;

} // namespace Message

/// @brief 文字列
namespace Message {

    PACK(template <size_t N> struct String {
        char string[N];
        void show() const {
            for (auto &&it : string) {
                Serial.print(it);
            }
            Serial.print('\t');
        }
    });

}

/// @brief 表示系
namespace Message {

    PACK(struct Lcd1602 {
        String<16> top;
        String<16> bottom;
        void show() const {
            Serial.print("Lcd1602: \n");
            top.show();
            Serial.print('\n');
            bottom.show();
            Serial.print('\n');
        }
    });

    PACK(struct Lcd2004 {
        String<20> line1;
        String<20> line2;
        String<20> line3;
        String<20> line4;
        void show() const {
            Serial.print("Lcd2004: \n");
            line1.show();
            Serial.print('\n');
            line2.show();
            Serial.print('\n');
            line3.show();
            Serial.print('\n');
            line4.show();
            Serial.print('\n');
        }
    });

} // namespace Message

/// @brief 色
namespace Message {

    /// @brief RGB
    PACK(struct RGB {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        void show() const {
            Serial.print("RGB: ");

            Serial.print(r);
            Serial.print('\t');

            Serial.print(g);
            Serial.print('\t');

            Serial.print(b);
            Serial.print('\t');
        }
    });

    /// @brief HSV
    PACK(struct HSV {
        uint8_t h;
        uint8_t s;
        uint8_t v;
        void show() const {
            Serial.print("HSV: ");

            Serial.print(h);
            Serial.print('\t');

            Serial.print(s);
            Serial.print('\t');

            Serial.print(v);
            Serial.print('\t');
        }
    });

    /// @brief HSL
    PACK(struct HSL {
        uint8_t h;
        uint8_t s;
        uint8_t l;
        void show() const {
            Serial.print("HSL: ");

            Serial.print(h);
            Serial.print('\t');

            Serial.print(s);
            Serial.print('\t');

            Serial.print(l);
            Serial.print('\t');
        }
    });

} // namespace Message

/// @brief コントローラー
namespace Message {
    PACK(struct ControllerPS4 {
        uint8_t isConnected : 1;

        uint8_t triangle : 1;
        uint8_t circle : 1;
        uint8_t cross : 1;
        uint8_t square : 1;

        uint8_t l1 : 1;
        uint8_t r1 : 1;
        uint8_t l2 : 1;
        uint8_t r2 : 1;
        uint8_t l3 : 1;
        uint8_t r3 : 1;

        uint8_t up : 1;
        uint8_t right : 1;
        uint8_t down : 1;
        uint8_t left : 1;

        uint8_t analogRightX;
        uint8_t analogRightY;
        uint8_t analogLeftX;
        uint8_t analogLeftY;
        uint8_t analogL2;
        uint8_t analogR2;

        uint8_t share : 1;
        uint8_t start : 1;
        uint8_t touch : 1;
    });

    /// todo
    PACK(struct ControllerPS5 {
        uint8_t isConnected : 1;

        uint8_t triangle : 1;
        uint8_t circle : 1;
        uint8_t cross : 1;
        uint8_t square : 1;

        uint8_t l1 : 1;
        uint8_t r1 : 1;
        uint8_t l2 : 1;
        uint8_t r2 : 1;
        uint8_t l3 : 1;
        uint8_t r3 : 1;

        uint8_t up : 1;
        uint8_t right : 1;
        uint8_t down : 1;
        uint8_t left : 1;

        uint8_t analogRightX;
        uint8_t analogRightY;
        uint8_t analogLeftX;
        uint8_t analogLeftY;
        uint8_t analogL2;
        uint8_t analogR2;

        uint8_t create : 1;
        uint8_t option : 1;
        uint8_t touch : 1;

        uint8_t mic : 1;
    });

} // namespace Message
