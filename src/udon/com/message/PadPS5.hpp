//-----------------------------------------------
//
//	UdonLibrary
//
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  通信用メッセージ
//
//-----------------------------------------------

#pragma once

#include <udon/utility/Parsable.hpp>

#include <stdint.h>

namespace udon
{
    namespace message
    {

        /// @brief PS5コントローラーのボタン情報
        /// @note 参照: https://controller.dl.playstation.net/controller/lang/jp/DS_partnames.html
        struct PadPS5
        {
            bool isConnected;    // 接続状態

            bool triangle;    // ▵
            bool circle;      // ○
            bool cross;       // ×
            bool square;      // □

            bool up;       // ↑
            bool right;    // →
            bool down;     // ↓
            bool left;     // ←

            bool l1;
            bool r1;

            bool l2;
            bool r2;

            bool l3;    // 左スティック押し込み
            bool r3;    // 右スティック押し込み

            bool create;    // 左上 \|/ ボタン
            bool option;    // 右上  ≡  ボタン

            bool touch;    // タッチパッド

            bool mic;    // ミュートボタン
            bool ps;     // PSボタン

            int8_t analogRightX;
            int8_t analogRightY;

            int8_t analogLeftX;
            int8_t analogLeftY;

            void clear()
            {
                isConnected = false;

                triangle = false;
                circle   = false;
                cross    = false;
                square   = false;

                up    = false;
                right = false;
                down  = false;
                left  = false;

                l1 = false;
                r1 = false;

                l2 = false;
                r2 = false;

                l3 = false;
                r3 = false;

                create = false;
                option = false;

                touch = false;
                mic   = false;
                ps    = false;

                analogRightX = 0;
                analogRightY = 0;

                analogLeftX = 0;
                analogLeftY = 0;
            }

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                if (isConnected)
                {
                    Serial.print("connected ");
                }
                else
                {
                    Serial.print("disconnected ");
                }

                Serial.print(triangle), Serial.print(' ');
                Serial.print(circle), Serial.print(' ');
                Serial.print(cross), Serial.print(' ');
                Serial.print(square), Serial.print(' ');

                Serial.print(up), Serial.print(' ');
                Serial.print(right), Serial.print(' ');
                Serial.print(down), Serial.print(' ');
                Serial.print(left), Serial.print(' ');

                Serial.print(l1), Serial.print(' ');
                Serial.print(r1), Serial.print(' ');
                Serial.print(l2), Serial.print(' ');
                Serial.print(r2), Serial.print(' ');
                Serial.print(l3), Serial.print(' ');
                Serial.print(r3), Serial.print(' ');

                Serial.print(create), Serial.print(' ');
                Serial.print(option), Serial.print(' ');
                Serial.print(touch), Serial.print(' ');
                Serial.print(mic), Serial.print(' ');
                Serial.print(ps), Serial.print(' ');
                
                Serial.print(analogRightX), Serial.print('\t');
                Serial.print(analogRightY), Serial.print('\t');
                Serial.print(analogLeftX), Serial.print('\t');
                Serial.print(analogLeftY), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(
                isConnected,
                triangle, circle, cross, square,
                up, right, down, left,
                l1, r1, l2, r2, l3, r3,
                create, option, touch, mic, ps,
                analogRightX, analogRightY, analogLeftX, analogLeftY);
        };
    }    // namespace message
}    // namespace udon
