//
//    OpenSiv3D用 Serial 送信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//
//    Sender --[Serial]--> Receiver
//    ^^^^^^
//

#pragma once

#include <Siv3D/Serial.hpp>
#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    /// @brief OpenSiv3D用 Serial 送信クラス
    /// @tparam Message メッセージ型
    template <typename Message>
    class SivSerialWriter
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        s3d::Serial& serial;

    public:

        /// @brief コンストラクタ
        /// @param bus Serialバス
        SivSerialWriter(s3d::Serial& bus)
            : serial(bus)
        {
        }

        /// @brief メッセージの送信
        /// @param message 送信メッセージ
        void setMessage(const Message& message)
        {
            if (serial.isOpen())
            {
                const auto packed = Udon::Serialize(message);
                serial.write(packed.data(), packed.size());
            }
        }
    };
}    // namespace Udon
