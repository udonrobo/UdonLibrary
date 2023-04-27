#pragma once

#include <algorithm>    // std::move
#include <udon/algorithm/Button.hpp>
#include <udon/traits/HasMember.hpp>
#include <udon/types/Position.hpp>
#include <udon/types/Vector2D.hpp>
#include <udon/message/PadPS5.hpp>

namespace udon
{

    /// @brief S5コントローラー受信クラス
    template<template<typename> typename Reader>
    class PadPS5
    {

        /// @brief 受信クラスの型
        using reader_type = Reader<udon::message::PadPS5>;

        /// @brief 受信クラス
        reader_type reader;

        /// @brief 接続状態
        bool isConnected = false;

        /// @brief ボタン [true/false]
        udon::Input triangle;
        udon::Input circle;
        udon::Input cross;
        udon::Input square;
        udon::Input up;
        udon::Input right;
        udon::Input down;
        udon::Input left;
        udon::Input l1;
        udon::Input r1;
        udon::Input l2;
        udon::Input r2;
        udon::Input l3;
        udon::Input r3;
        udon::Input create;
        udon::Input option;
        udon::Input touch;
        udon::Input mic;

        /// @brief アナログスティック [-255~255]
        udon::Vec2 rightStick;
        udon::Vec2 leftStick;

        /// @brief アナログトリガー [-255~255]
        uint8_t analogL2 = 0;
        uint8_t analogR2 = 0;

    public:

        PadPS5(reader_type&& reader)
            : reader(std::move(reader))
        {
        }

        /// @brief 更新
        /// @tparam T
        /// @return
        template<typename T = reader_type>
        auto update() -> typename std::enable_if<udon::has_member_function_update_v<T>>::type
        {
            reader.update();
            _update();
        }
        /// @brief 更新
        template<typename T = reader_type>
        auto update() -> typename std::enable_if<!udon::has_member_function_update_v<T>>::type
        {
            _update();
        }

        /// @brief コントローラーが接続されているか
        operator bool () const
        {
            return isConnected;
        }

        /// @brief トライアングルボタン
        /// @return
        udon::Button getTriangle() const
        {
            return triangle;
        }
        /// @brief サークルボタン
        /// @return
        udon::Button getCircle() const
        {
            return circle;
        }
        /// @brief クロスボタン
        /// @return
        udon::Button getCross() const
        {
            return cross;
        }
        /// @brief スクエアボタン
        /// @return
        udon::Button getSquare() const
        {
            return square;
        }
        /// @brief 十字キー上
        /// @return
        udon::Button getUp() const
        {
            return up;
        }
        /// @brief 十字キー右
        /// @return
        udon::Button getRight() const
        {
            return right;
        }
        /// @brief 十字キー下
        /// @return
        udon::Button getDown() const
        {
            return down;
        }
        /// @brief 十字キー左
        /// @return
        udon::Button getLeft() const
        {
            return left;
        }
        /// @brief L1ボタン
        /// @return
        udon::Button getL1() const
        {
            return l1;
        }
        /// @brief R1ボタン
        /// @return
        udon::Button getR1() const
        {
            return r1;
        }
        /// @brief L2ボタン
        /// @return
        udon::Button getL2() const
        {
            return l2;
        }
        /// @brief R2ボタン
        /// @return
        udon::Button getR2() const
        {
            return r2;
        }
        /// @brief 左スティック押し込み
        /// @return
        udon::Button getL3() const
        {
            return l3;
        }
        /// @brief 右スティック押し込み
        /// @return
        udon::Button getR3() const
        {
            return r3;
        }
        /// @brief クリエイトボタン(左上ボタン)
        /// @return
        udon::Button getCreate() const
        {
            return create;
        }
        /// @brief オプションボタン(右上ボタン)
        /// @return
        udon::Button getOption() const
        {
            return option;
        }
        /// @brief タッチパッドボタン
        /// @return
        udon::Button getTouch() const
        {
            return touch;
        }
        /// @brief マイクボタン
        /// @return
        udon::Button getMic() const
        {
            return mic;
        }
        /// @brief 左スティック
        /// @return
        udon::Vec2 getLeftStick() const
        {
            return leftStick;
        }
        /// @brief 右スティック
        /// @return
        udon::Vec2 getRightStick() const
        {
            return rightStick;
        }
        /// @brief ロボットの移動に必要なスティックの情報を取得
        /// @remark 左スティックから移動成分、右スティックX軸から旋回成分を取得
        /// @return
        udon::Pos getStick() const
        {
            return { leftStick, rightStick.x };
        }
        /// @brief L2アナログ
        /// @return
        uint8_t getL2Analog() const
        {
            return analogL2;
        }
        /// @brief R2アナログ
        /// @return
        uint8_t getR2Analog() const
        {
            return analogR2;
        }

    private:
        void _update()
        {
            const auto msg = reader.getMessage();

            isConnected = msg.isConnected;

            triangle.update(msg.triangle);
            circle.update(msg.circle);
            cross.update(msg.cross);
            square.update(msg.square);
            up.update(msg.up);
            right.update(msg.right);
            down.update(msg.down);
            left.update(msg.left);
            l1.update(msg.l1);
            r1.update(msg.r1);
            l2.update(msg.l2);
            r2.update(msg.r2);
            l3.update(msg.l3);
            r3.update(msg.r3);
            create.update(msg.create);
            option.update(msg.option);
            touch.update(msg.touch);
            mic.update(msg.mic);

            // todo 通信時のスティックの値の規格決め[0~255?][-255~255?]
            // todo デッドゾーン処理
            rightStick = { (double)msg.analogRightX, (double)msg.analogRightY };
            leftStick  = { (double)msg.analogLeftX,  (double)msg.analogLeftY  };

            analogL2 = msg.analogL2;
            analogR2 = msg.analogR2;
        }
    };
}    // namespace udon

#include <udon/com/can/CanReader.hpp>
#include <udon/com/uart/UartReader.hpp>
#include <udon/com/i2c/I2cMasterReader.hpp>

namespace udon
{

    /// @brief CAN通信経由
    using CanPadPS5 = udon::PadPS5<udon::CanReader>;

    /// @brief UART通信経由
    using UartPadPS5 = udon::PadPS5<udon::UartReader>;

    /// @brief I2C通信経由
    using I2cPadPS5 = udon::PadPS5<udon::I2cMasterReader>;

}    // namespace udon
