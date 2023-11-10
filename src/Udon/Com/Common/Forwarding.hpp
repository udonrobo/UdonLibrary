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

#include <Udon/Common/Show.hpp>
#include <Udon/Traits/HasMemberFunction.hpp>
#include <Udon/Traits/IsReader.hpp>
#include <Udon/Traits/IsWriter.hpp>

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

        static_assert(Udon::Traits::IsReader<Reader>::value, "Reader must be reader.");
        static_assert(Udon::Traits::IsWriter<Writer>::value, "Writer must be writer.");
        
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
            Udon::Traits::MaybeInvokeBegin(reader);
            Udon::Traits::MaybeInvokeBegin(writer);
        }

        /// @brief 更新
        /// @details 受信クラス、または送信クラスに update メンバ関数がある場合、呼び出す
        void update()
        {
            Udon::Traits::MaybeInvokeUpdate(reader);
            if (const auto message = reader.getMessage())
            {
                writer.setMessage(*message);
            }
            else
            {
                writer.setErrorMessage();
            }
            Udon::Traits::MaybeInvokeUpdate(writer);
        }

        /// @brief 通信状態を表示する
        /// @details 受信クラス、または送信クラスに show メンバ関数がある場合、呼び出す
        void show()
        {
            Udon::Show(F("read: "));
            Udon::Traits::MaybeInvokeShow(reader);
            Udon::Show(F("  write: "));
            Udon::Traits::MaybeInvokeShow(writer);
            Udon::Show(F("  "));
        }

        /// @brief 通信状態を表示する
        /// @details 受信クラス、または送信クラスに showRaw メンバ関数がある場合、呼び出す
        void showRaw()
        {
            Udon::Show(F("read: "));
            Udon::Traits::MaybeInvokeShowRaw(reader);
            Udon::Show(F("  write: "));
            Udon::Traits::MaybeInvokeShowRaw(writer);
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
