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

#include <Udon/Traits/MaybeInvoke.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
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
            Udon::MaybeInvoke_begin(reader);
            Udon::MaybeInvoke_begin(writer);
        }

        /// @brief 更新
        /// @details 受信クラス、または送信クラスに update メンバ関数がある場合、呼び出す
        void update()
        {
            Udon::MaybeInvoke_update(reader);
            if (const auto message = reader.getMessage())
            {
                writer.setMessage(*message);
            }
            Udon::MaybeInvoke_update(writer);
        }

        /// @brief 通信状態を表示する
        /// @details 受信クラス、または送信クラスに show メンバ関数がある場合、呼び出す
        void show()
        {
            Udon::Show(F("read: "));
            Udon::MaybeInvoke_show(reader);
            Udon::Show(F("  write: "));
            Udon::MaybeInvoke_show(writer);
            Udon::Show(F("  "));
        }

        /// @brief 通信状態を表示する
        /// @details 受信クラス、または送信クラスに showRaw メンバ関数がある場合、呼び出す
        void showRaw()
        {
            Udon::Show(F("read: "));
            Udon::MaybeInvoke_showRaw(reader);
            Udon::Show(F("  write: "));
            Udon::MaybeInvoke_showRaw(writer);
            Udon::Show(F("  "));
        }

        /// @brief 受信クラスの参照を取得する
        /// @return 受信クラスの参照
        reader_type& getReader()
        {
            return reader;
        }

        /// @brief 送信クラスの参照を取得する
        /// @return 送信クラスの参照
        writer_type& getWriter()
        {
            return writer;
        }
        
    };
}    // namespace Udon
