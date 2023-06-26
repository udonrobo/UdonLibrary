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
//  PS5コントローラー受信クラス
//
//-----------------------------------------------

#pragma once

#include <udon/algorithm/Button.hpp>
#include <udon/com/message/PadPS5.hpp>
#include <udon/traits/MaybeInvoke.hpp>
#include <udon/types/Position.hpp>
#include <udon/types/Vector2D.hpp>

#include <algorithm>

namespace udon
{

    /// @brief PS5コントローラー受信クラス
    /// @tparam Reader 受信クラス
    /// @details
    /// Readerはテンプレート引数にudon::message::PadPS5を指定できる必要があります。
    /// 以下のメンバ関数を持つ必要があります。
    ///   - Message getMessage()
    /// 以下のメンバ関数を持つ場合、自動的に呼び出されます。
    ///   - void begin()
    ///   - void update()
    /// 以下はReaderクラスの最小実装例です。
    /// @code
    /// template<typename Message>
    /// class Reader
    /// {
    /// public:
    ///     // 任意
    ///     void begin()
    ///     {
    ///     }
    ///     // 任意
    ///     void update()
    ///     {
    ///     }
    ///     // 必須
    ///     Message getMessage() const
    ///     {
    ///         Message message;
    ///         return message;
    ///     }
    /// };
    /// @endcode
    template <template <typename> typename Reader>
    class PadPS5
    {

        /// @brief 受信クラスの型
        using reader_type = Reader<message::PadPS5>;

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
        udon::Input ps;

        /// @brief アナログスティック [-255~255]
        udon::Vec2 rightStick;
        udon::Vec2 leftStick;

    public:
        /// @brief デフォルトコンストラクタ
        /// @remark 受信クラスがデフォルトコンストラクタを持つ場合インスタンス化されます。
        /// @tparam T
        template <typename T = reader_type, typename std::enable_if<std::is_default_constructible<T>::value>::type* = nullptr>
        PadPS5()
            : reader()
        {
        }

        /// @brief コンストラクタ
        /// @param reader 受信クラス
        PadPS5(reader_type&& reader)
            : reader(std::move(reader))
        {
        }

        /// @brief 開始
        /// @remark 受信クラスがbegin()を持つ場合インスタンス化されます。
        /// @tparam T
        /// @return
        template <typename T = reader_type>
        auto begin()
            -> typename std::enable_if<udon::has_member_function_begin<T>::value>::type
        {
            reader.begin();
        }

        /// @brief 更新
        /// @remark 受信クラスがupdate()を持つ場合インスタンス化されます。
        /// @tparam T
        /// @return
        void update();

        /// @brief コントローラーが接続されているか
        explicit operator bool() const
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
        /// @brief PSボタン
        /// @return
        udon::Button getPs() const
        {
            return ps;
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
        /// @brief ロボットの移動に必要なスティックの情報 udon::Positionオブジェクト {{x,y},turn} を取得
        /// @remark 左スティックから移動成分、右スティックX軸から旋回成分を取得
        /// @return
        udon::Pos getMoveInfo() const
        {
            return { leftStick, rightStick.x };
        }

        void show() const
        {
            udon::MaybeInvoke_show(reader);
        }

        void showRaw() const
        {
            udon::MaybeInvoke_showRaw(reader);
        }
    };

    // detail

    /// @brief 更新
    /// @remark 受信クラスがupdate()を持つ場合インスタンス化されます。
    /// @tparam T
    /// @return
    template <template <typename> typename Reader>
    void PadPS5<Reader>::update()
    {
        udon::MaybeInvoke_update(reader);

        if (auto&& message = reader.getMessage())
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
                return raw * 2 + 1;  // (raw + 128) * 2 - 255
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

}    // namespace udon
