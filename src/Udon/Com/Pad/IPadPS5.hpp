//
//    PS5コントローラー受信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Controller --> Sender --> Master
//                              ^^^^^^
//

#pragma once

#include <Udon/Algorithm/Input.hpp>
#include <Udon/Com/Message/PadPS5.hpp>
#include <Udon/Types/Position.hpp>
#include <Udon/Com/Message.hpp>

namespace Udon
{

    /// @brief PS5コントローラーインターフェース
    /// @details このクラスを継承し、updateFromMessageOpt にメッセージを渡すか、updateFromRawData に各値をセットし更新することで、コントローラーの情報を取得できます。
    class IPadPS5
    {

        /// @brief 接続状態
        bool m_isConnected = false;

        /// @brief ボタン
        Udon::Input m_triangle;
        Udon::Input m_circle;
        Udon::Input m_cross;
        Udon::Input m_square;
        Udon::Input m_up;
        Udon::Input m_right;
        Udon::Input m_down;
        Udon::Input m_left;
        Udon::Input m_l1;
        Udon::Input m_r1;
        Udon::Input m_l2;
        Udon::Input m_r2;
        Udon::Input m_l3;
        Udon::Input m_r3;
        Udon::Input m_create;
        Udon::Input m_option;
        Udon::Input m_touch;
        Udon::Input m_mic;
        Udon::Input m_ps;

        /// @brief アナログスティック [-255~255]
        Udon::Vec2 m_rightStick;
        Udon::Vec2 m_leftStick;

    public:
        /// @brief コントローラーが接続されているか
        explicit operator bool() const { return m_isConnected; }

        /// @brief トライアングルボタン
        Udon::Button getTriangle() const { return m_triangle; }

        /// @brief サークルボタン
        Udon::Button getCircle() const { return m_circle; }

        /// @brief クロスボタン
        Udon::Button getCross() const { return m_cross; }

        /// @brief スクエアボタン
        Udon::Button getSquare() const { return m_square; }

        /// @brief 十字キー上
        Udon::Button getUp() const { return m_up; }

        /// @brief 十字キー右
        Udon::Button getRight() const { return m_right; }

        /// @brief 十字キー下
        Udon::Button getDown() const { return m_down; }

        /// @brief 十字キー左
        Udon::Button getLeft() const { return m_left; }

        /// @brief L1ボタン
        Udon::Button getL1() const { return m_l1; }

        /// @brief R1ボタン
        Udon::Button getR1() const { return m_r1; }

        /// @brief L2ボタン
        Udon::Button getL2() const { return m_l2; }

        /// @brief R2ボタン
        Udon::Button getR2() const { return m_r2; }

        /// @brief 左スティック押し込み
        Udon::Button getL3() const { return m_l3; }

        /// @brief 右スティック押し込み
        Udon::Button getR3() const { return m_r3; }

        /// @brief クリエイトボタン(左上ボタン)
        Udon::Button getCreate() const { return m_create; }

        /// @brief オプションボタン(右上ボタン)
        Udon::Button getOption() const { return m_option; }

        /// @brief タッチパッドボタン
        Udon::Button getTouch() const { return m_touch; }

        /// @brief マイクボタン
        Udon::Button getMic() const { return m_mic; }

        /// @brief PSボタン
        Udon::Button getPs() const { return m_ps; }

        /// @brief 左スティック
        Udon::Vec2 getLeftStick() const { return m_leftStick; }

        /// @brief 右スティック
        Udon::Vec2 getRightStick() const { return m_rightStick; }

        /// @brief ロボットの移動に必要なスティックの情報 Udon::Positionオブジェクト {{x,y},turn} を取得
        /// @remark 左スティックから移動成分、右スティックX軸から旋回成分を取得
        Udon::Pos getMoveInfo() const { return { m_leftStick, m_rightStick.x }; }

        Udon::Message::PadPS5 toMessage() const
        {
            return {
                m_isConnected,
                m_triangle.press(),
                m_circle.press(),
                m_cross.press(),
                m_square.press(),

                m_up.press(),
                m_right.press(),
                m_down.press(),
                m_left.press(),

                m_l1.press(),
                m_r1.press(),
                m_l2.press(),
                m_r2.press(),
                m_l3.press(),
                m_r3.press(),

                m_create.press(),
                m_option.press(),
                m_touch.press(),
                m_mic.press(),
                m_ps.press(),

                static_cast<int8_t>(m_rightStick.x / 2),
                static_cast<int8_t>(m_rightStick.y / 2),
                static_cast<int8_t>(m_leftStick.x / 2),
                static_cast<int8_t>(m_leftStick.y / 2),
            };
        }

    protected:
        /// @brief 継承したクラスが呼び出す関数
        /// @param message メッセージ
        void updateFromMessageOpt(const Udon::Optional<Udon::Message::PadPS5>& message);

        /// @brief 継承したクラスが呼び出す関数
        /// @param isConnected  コントローラーが接続されているか
        /// @param analogLeftX  左のアナログスティックX軸 [-255~255]
        /// @param analogLeftY  左のアナログスティックY軸 [-255~255]
        /// @param analogRightX 右のアナログスティックX軸 [-255~255]
        /// @param analogRightY 右のアナログスティックY軸 [-255~255]
        void updateFromRawData(bool   isConnected,
                               bool   triangle,
                               bool   circle,
                               bool   cross,
                               bool   square,
                               bool   up,
                               bool   right,
                               bool   down,
                               bool   left,
                               bool   l1,
                               bool   r1,
                               bool   l2,
                               bool   r2,
                               bool   l3,
                               bool   r3,
                               bool   create,
                               bool   option,
                               bool   touch,
                               bool   mic,
                               bool   ps,
                               double analogLeftX,
                               double analogLeftY,
                               double analogRightX,
                               double analogRightY);

        /// @brief ボタンの状態をすべてクリア
        void clear()
        {
            m_isConnected = false;

            m_triangle.update(false);
            m_circle.update(false);
            m_cross.update(false);
            m_square.update(false);

            m_up.update(false);
            m_right.update(false);
            m_down.update(false);
            m_left.update(false);

            m_l1.update(false);
            m_r1.update(false);
            m_l2.update(false);
            m_r2.update(false);
            m_l3.update(false);
            m_r3.update(false);

            m_create.update(false);
            m_option.update(false);
            m_touch.update(false);
            m_mic.update(false);

            m_leftStick.clear();
            m_rightStick.clear();
        }

    private:
        /// @brief デッドゾーンをカットする
        static double CutDeadZone(double value, double deadZone)
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
    };

    inline void IPadPS5::updateFromMessageOpt(const Udon::Optional<Udon::Message::PadPS5>& message)
    {
        if (not message && not message->isConnected)
        {
            clear();
            return;
        }

        m_isConnected = true;

        m_triangle.update(message->triangle);
        m_circle.update(message->circle);
        m_cross.update(message->cross);
        m_square.update(message->square);

        m_up.update(message->up);
        m_right.update(message->right);
        m_down.update(message->down);
        m_left.update(message->left);

        m_l1.update(message->l1);
        m_r1.update(message->r1);
        m_l2.update(message->l2);
        m_r2.update(message->r2);
        m_l3.update(message->l3);
        m_r3.update(message->r3);

        m_create.update(message->create);
        m_option.update(message->option);
        m_touch.update(message->touch);
        m_mic.update(message->mic);
        m_ps.update(message->ps);

        // -128 ~ 127(int8_t) -> -255 ~ 255(int16_t)
        const auto decodeStick = [](int8_t raw) -> int16_t
        {
            return raw * 2 + 1;    // (raw + 128) * 2 - 255
        };

        m_leftStick = {
            CutDeadZone(decodeStick(message->analogLeftX), 20),
            CutDeadZone(decodeStick(message->analogLeftY), 20),
        };
        m_rightStick = {
            CutDeadZone(decodeStick(message->analogRightX), 20),
            CutDeadZone(decodeStick(message->analogRightY), 20),
        };
    }

    inline void IPadPS5::updateFromRawData(bool   isConnected,
                                           bool   triangle,
                                           bool   circle,
                                           bool   cross,
                                           bool   square,
                                           bool   up,
                                           bool   right,
                                           bool   down,
                                           bool   left,
                                           bool   l1,
                                           bool   r1,
                                           bool   l2,
                                           bool   r2,
                                           bool   l3,
                                           bool   r3,
                                           bool   create,
                                           bool   option,
                                           bool   touch,
                                           bool   mic,
                                           bool   ps,
                                           double analogLeftX,
                                           double analogLeftY,
                                           double analogRightX,
                                           double analogRightY)
    {

        if (not isConnected)
        {
            clear();
            return;
        }

        m_isConnected = true;

        m_triangle.update(triangle);
        m_circle.update(circle);
        m_cross.update(cross);
        m_square.update(square);

        m_up.update(up);
        m_right.update(right);
        m_down.update(down);
        m_left.update(left);

        m_l1.update(l1);
        m_r1.update(r1);
        m_l2.update(l2);
        m_r2.update(r2);
        m_l3.update(l3);
        m_r3.update(r3);

        m_create.update(create);
        m_option.update(option);
        m_touch.update(touch);
        m_mic.update(mic);
        m_ps.update(ps);

        m_leftStick = {
            CutDeadZone(analogLeftX, 20),
            CutDeadZone(analogLeftY, 20),
        };
        m_rightStick = {
            CutDeadZone(analogRightX, 20),
            CutDeadZone(analogRightY, 20),
        };
    }

}    // namespace Udon
