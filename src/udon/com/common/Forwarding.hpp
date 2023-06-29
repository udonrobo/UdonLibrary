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
//    通信転送クラス
//
//    Sender --[CAN ect.]--> Forwarding --[UART etc.]--> Receiver
//                           ^^^^^^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include <udon/traits/MaybeInvoke.hpp>
#include <udon/utility/Show.hpp>

namespace udon
{

    /// @brief 異なる通信間にメッセージを転送するクラス
    /// @tparam Reader 受信クラス
    /// @tparam Writer 送信クラス
    /// @tparam Message メッセージ型
    template <
        template <typename> typename Reader,
        template <typename> typename Writer,
        typename Message>
    class Forwarding
    {

        /// @brief 受信クラスの型
        using reader_type = Reader<Message>;

        /// @brief 送信クラスの型
        using writer_type = Writer<Message>;

        /// @brief 受信クラス
        reader_type reader;

        /// @brief 送信クラス
        writer_type writer;

    public:
        /// @brief コンストラクタ
        /// @param reader 受信クラス
        /// @param writer 送信クラス
        Forwarding(reader_type&& reader, writer_type&& writer)
            : reader(std::move(reader))
            , writer(std::move(writer))
        {
        }

        /// @brief 開始
        /// @details 受信クラス、または送信クラスに begin メンバ関数がある場合、呼び出す
        void begin()
        {
            udon::MaybeInvoke_begin(reader);
            udon::MaybeInvoke_begin(writer);
        }

        /// @brief 更新
        /// @details 受信クラス、または送信クラスに update メンバ関数がある場合、呼び出す
        void update()
        {
            udon::MaybeInvoke_update(reader);
            if (const auto message = reader.getMessage())
            {
                writer.setMessage(*message);
            }
            udon::MaybeInvoke_update(writer);
        }

        /// @brief 通信状態を表示する
        /// @details 受信クラス、または送信クラスに show メンバ関数がある場合、呼び出す
        void show()
        {
            udon::Show(F("read: "));
            udon::MaybeInvoke_show(reader);
            udon::Show(F("write: "));
            udon::MaybeInvoke_show(writer);
        }

        /// @brief 通信状態を表示する
        /// @details 受信クラス、または送信クラスに showRaw メンバ関数がある場合、呼び出す
        void showRaw()
        {
            udon::Show(F("read: "));
            udon::MaybeInvoke_showRaw(reader);
            udon::Show(F("write: "));
            udon::MaybeInvoke_showRaw(writer);
        }
    };
}    // namespace udon
