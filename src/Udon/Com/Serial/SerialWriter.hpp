//
//    Serial 送信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//
//    Sender --[Serial]--> Receiver
//    ^^^^^^
//

#pragma once

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    /// @brief Serial 送信クラス
    /// @tparam Message メッセージ型
    template <typename Message>
    class SerialWriter
    {

        Stream& serial;

        Message message;

    public:
        /// @brief コンストラクタ
        SerialWriter(Stream& serial) noexcept
            : serial(serial)
            , message()
        {
        }

        /// @brief 送信内容を更新
        /// @param rhs 送信内容
        void setMessage(const Message& rhs)
        {
            message = rhs;
            for (auto&& it : Udon::Serialize(message))
            {
                serial.write(it);
            }
        }

        /// @brief 送信内容を表示
        void show() const
        {
            Udon::Show(message);
        }

        /// @brief 生の送信内容を表示
        void showRaw() const
        {
            for (auto&& it : Udon::Serialize(message))
            {
                Serial.print(it);
                Serial.print(' ');
            }
        }
    };

}    // namespace Udon
