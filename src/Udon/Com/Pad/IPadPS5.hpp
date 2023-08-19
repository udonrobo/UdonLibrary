//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    PS5コントローラー受信クラス
//
//    Controller --> Sender --> Master
//                              ^^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Algorithm/Button.hpp>
#include <Udon/Com/Message/PadPS5.hpp>
#include <Udon/Traits/MaybeInvoke.hpp>
#include <Udon/Types/Position.hpp>
#include <Udon/Types/Vector2D.hpp>

#include <algorithm>

namespace Udon
{

    /// @brief PS5コントローラーインターフェース
    /// @details このクラスを継承し、update 関数にメッセージを渡すことで、コントローラーの情報を取得できます。
    class IPadPS5
    {

        /// @brief 接続状態
        bool isConnected = false;

        /// @brief ボタン [true/false]
        Udon::Input triangle;
        Udon::Input circle;
        Udon::Input cross;
        Udon::Input square;
        Udon::Input up;
        Udon::Input right;
        Udon::Input down;
        Udon::Input left;
        Udon::Input l1;
        Udon::Input r1;
        Udon::Input l2;
        Udon::Input r2;
        Udon::Input l3;
        Udon::Input r3;
        Udon::Input create;
        Udon::Input option;
        Udon::Input touch;
        Udon::Input mic;
        Udon::Input ps;

        /// @brief アナログスティック [-255~255]
        Udon::Vec2 rightStick;
        Udon::Vec2 leftStick;

    protected:
        /// @brief 継承したクラスが呼び出す関数
        /// @param message メッセージ
        void update(const Udon::Optional<Udon::Message::PadPS5>& message);

    public:
        /// @brief コントローラーが接続されているか
        explicit operator bool() const
        {
            return isConnected;
        }

        /// @brief トライアングルボタン
        /// @return
        Udon::Button getTriangle() const
        {
            return triangle;
        }
        /// @brief サークルボタン
        /// @return
        Udon::Button getCircle() const
        {
            return circle;
        }
        /// @brief クロスボタン
        /// @return
        Udon::Button getCross() const
        {
            return cross;
        }
        /// @brief スクエアボタン
        /// @return
        Udon::Button getSquare() const
        {
            return square;
        }
        /// @brief 十字キー上
        /// @return
        Udon::Button getUp() const
        {
            return up;
        }
        /// @brief 十字キー右
        /// @return
        Udon::Button getRight() const
        {
            return right;
        }
        /// @brief 十字キー下
        /// @return
        Udon::Button getDown() const
        {
            return down;
        }
        /// @brief 十字キー左
        /// @return
        Udon::Button getLeft() const
        {
            return left;
        }
        /// @brief L1ボタン
        /// @return
        Udon::Button getL1() const
        {
            return l1;
        }
        /// @brief R1ボタン
        /// @return
        Udon::Button getR1() const
        {
            return r1;
        }
        /// @brief L2ボタン
        /// @return
        Udon::Button getL2() const
        {
            return l2;
        }
        /// @brief R2ボタン
        /// @return
        Udon::Button getR2() const
        {
            return r2;
        }
        /// @brief 左スティック押し込み
        /// @return
        Udon::Button getL3() const
        {
            return l3;
        }
        /// @brief 右スティック押し込み
        /// @return
        Udon::Button getR3() const
        {
            return r3;
        }
        /// @brief クリエイトボタン(左上ボタン)
        /// @return
        Udon::Button getCreate() const
        {
            return create;
        }
        /// @brief オプションボタン(右上ボタン)
        /// @return
        Udon::Button getOption() const
        {
            return option;
        }
        /// @brief タッチパッドボタン
        /// @return
        Udon::Button getTouch() const
        {
            return touch;
        }
        /// @brief マイクボタン
        /// @return
        Udon::Button getMic() const
        {
            return mic;
        }
        /// @brief PSボタン
        /// @return
        Udon::Button getPs() const
        {
            return ps;
        }
        /// @brief 左スティック
        /// @return
        Udon::Vec2 getLeftStick() const
        {
            return leftStick;
        }
        /// @brief 右スティック
        /// @return
        Udon::Vec2 getRightStick() const
        {
            return rightStick;
        }
        /// @brief ロボットの移動に必要なスティックの情報 Udon::Positionオブジェクト {{x,y},turn} を取得
        /// @remark 左スティックから移動成分、右スティックX軸から旋回成分を取得
        /// @return
        Udon::Pos getMoveInfo() const
        {
            return { leftStick, rightStick.x };
        }

        void show() const
        {
            // TODO: 未実装
        }

        void showRaw() const
        {
            // TODO: 未実装
        }
    };

    inline void IPadPS5::update(const Udon::Optional<Udon::Message::PadPS5>& message)
    {
        if (message)
        {
            isConnected = message->isConnected;

            triangle.update(message->triangle);
            circle.update(message->circle);
            cross.update(message->cross);
            square.update(message->square);

            up.update(message->up);
            right.update(message->right);
            down.update(message->down);
            left.update(message->left);

            l1.update(message->l1);
            r1.update(message->r1);
            l2.update(message->l2);
            r2.update(message->r2);
            l3.update(message->l3);
            r3.update(message->r3);

            create.update(message->create);
            option.update(message->option);
            touch.update(message->touch);
            mic.update(message->mic);
            ps.update(message->ps);

            // -128 ~ 127(int8_t) -> -255 ~ 255(int16_t)
            const auto decodeStick = [](int8_t raw) -> int16_t
            {
                return raw * 2 + 1;    // (raw + 128) * 2 - 255
            };

            /// @brief デッドゾーン処理
            const auto cutDeadZone = [](double value, double deadZone) -> double
            {
                if (value > deadZone)
                {
                    return 255 * (value - deadZone) / (255 - deadZone);
                }
                else if (value < -deadZone)
                {
                    return 255 * (value + deadZone) / (255 - deadZone);
                }
                else
                {
                    return 0.0;
                }
            };

            leftStick = {
                cutDeadZone(decodeStick(message->analogLeftX), 20),
                cutDeadZone(decodeStick(message->analogLeftY), 20),
            };
            rightStick = {
                cutDeadZone(decodeStick(message->analogRightX), 20),
                cutDeadZone(decodeStick(message->analogRightY), 20),
            };
        }
        else
        {
            isConnected = false;

            triangle.update(false);
            circle.update(false);
            cross.update(false);
            square.update(false);

            up.update(false);
            right.update(false);
            down.update(false);
            left.update(false);

            l1.update(false);
            r1.update(false);
            l2.update(false);
            r2.update(false);
            l3.update(false);
            r3.update(false);

            create.update(false);
            option.update(false);
            touch.update(false);
            mic.update(false);

            leftStick.clear();
            rightStick.clear();
        }
    }

}    // namespace Udon
