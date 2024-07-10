#pragma once

#include <serial/serial.h>
#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{
    
    /// @brief ROS2からUART通信を送信するクラス
    template <typename Message>
    class Ros2UartWriter
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        serial::Serial& serial;

    public:
        /// @brief コンストラクタ
        /// @param bus UARTバス
        Ros2UartWriter(serial::Serial& bus)
            : serial(bus)
        {
        }

        /// @brief メッセージの送信
        /// @param message 送信メッセージ
        void setMessage(const Message& message)
        {
            uint8_t buffer[Size];
            Udon::Serialize(message, buffer);
            serial.write(buffer, Size);
        }
    };
}    // namespace Udon
