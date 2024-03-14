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

#include <Udon/Types/Position.hpp>
#include <Udon/Algorithm/Input.hpp>
#include <Udon/Com/Message.hpp>
#include <Udon/Com/Message/PadPS5.hpp>
#include <Udon/Traits/ReaderWriterTraits.hpp>

namespace Udon
{

    /// @brief PS5コントローラー
    /// @tparam Reader 受信クラス
    template <template <typename> typename Reader>
    class PadPS5
        : public Reader<Message::PadPS5>
    {
        static_assert(Traits::IsReader<Reader>::value, "Reader type must meet recipient requirements.");

    public:
        /// @brief 受信メッセージ型
        using MessageType = Message::PadPS5;

        /// @brief 受信クラス型
        using ReaderType = Reader<MessageType>;

        /// @brief コンストラクタ(受信クラスのコンストラクタを継承)
        using ReaderType::ReaderType;

        /// @brief コントローラーが接続されているか
        explicit operator bool() const noexcept { return isConnected; }

        /// @brief ▵ボタン
        Input getTriangle() const noexcept { return triangle; }

        /// @brief ○ボタン
        Input getCircle() const noexcept { return circle; }

        /// @brief ×ボタン
        Input getCross() const noexcept { return cross; }

        /// @brief □ボタン
        Input getSquare() const noexcept { return square; }

        /// @brief 十字キー上
        Input getUp() const noexcept { return up; }

        /// @brief 十字キー右
        Input getRight() const noexcept { return right; }

        /// @brief 十字キー下
        Input getDown() const noexcept { return down; }

        /// @brief 十字キー左
        Input getLeft() const noexcept { return left; }

        /// @brief L1ボタン
        Input getL1() const noexcept { return l1; }

        /// @brief R1ボタン
        Input getR1() const noexcept { return r1; }

        /// @brief L2ボタン
        Input getL2() const noexcept { return l2; }

        /// @brief R2ボタン
        Input getR2() const noexcept { return r2; }

        /// @brief 左スティック押し込み
        Input getL3() const noexcept { return l3; }

        /// @brief 右スティック押し込み
        Input getR3() const noexcept { return r3; }

        /// @brief クリエイトボタン(左上ボタン)
        Input getCreate() const noexcept { return create; }

        /// @brief オプションボタン(右上ボタン)
        Input getOption() const noexcept { return option; }

        /// @brief タッチパッドボタン
        Input getTouch() const noexcept { return touch; }

        /// @brief マイクボタン
        Input getMic() const noexcept { return mic; }

        /// @brief PSボタン
        Input getPs() const noexcept { return ps; }

        /// @brief 左スティック [x,y: -255~255]
        Vec2 getLeftStick() const noexcept { return leftStick; }

        /// @brief 右スティック [x,y: -255~255]
        Vec2 getRightStick() const noexcept { return rightStick; }

        /// @brief ロボットの移動に必要なスティックの情報 Udon::Stick オブジェクト {{x,y},turn} を取得
        /// @note 左スティックから移動成分、右スティックX軸から旋回成分を取得
        Stick getMoveInfo() const noexcept { return { leftStick, rightStick.x }; }

        /// @brief 更新
        void update() noexcept
        {
            // Traits::MaybeInvokeUpdate(*this);

            if (const auto message = ReaderType::getMessage())
            {
                isConnected = true;

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

                leftStick = {
                    CutDeadZone(decodeStick(message->analogLeftX), 20),
                    CutDeadZone(decodeStick(message->analogLeftY), 20),
                };
                rightStick = {
                    CutDeadZone(decodeStick(message->analogRightX), 20),
                    CutDeadZone(decodeStick(message->analogRightY), 20),
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

        /// @brief メッセージ構造体に変換
        MessageType toMessage() const
        {
            return {
                isConnected,
                triangle.press,
                circle.press,
                cross.press,
                square.press,

                up.press,
                right.press,
                down.press,
                left.press,

                l1.press,
                r1.press,
                l2.press,
                r2.press,
                l3.press,
                r3.press,

                create.press,
                option.press,
                touch.press,
                mic.press,
                ps.press,

                static_cast<int8_t>(rightStick.x / 2),
                static_cast<int8_t>(rightStick.y / 2),
                static_cast<int8_t>(leftStick.x / 2),
                static_cast<int8_t>(leftStick.y / 2),
            };
        }

    private:
        // 接続状態
        bool isConnected = false;

        // ボタン
        Input triangle;
        Input circle;
        Input cross;
        Input square;
        Input up;
        Input right;
        Input down;
        Input left;
        Input l1;
        Input r1;
        Input l2;
        Input r2;
        Input l3;
        Input r3;
        Input create;
        Input option;
        Input touch;
        Input mic;
        Input ps;

        // アナログスティック [-255~255]
        Vec2 rightStick;
        Vec2 leftStick;

        /// @brief デッドゾーンをカットする
        static double CutDeadZone(double value, uint8_t deadZone)
        {
            if (value > deadZone)
                return 255 * (value - deadZone) / (255 - deadZone);
            else if (value < -deadZone)
                return 255 * (value + deadZone) / (255 - deadZone);
            else
                return 0.0;
        };
    };

}    // namespace Udon
