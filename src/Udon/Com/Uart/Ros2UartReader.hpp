#pragma once

#include <serial/serial.h>
#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    /// @brief ROS2からUART通信を受信するクラス
    template <typename Message>
    class Ros2UartReader
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        serial::Serial& serial;

        std::vector<uint8_t> buffer;

    public:
        /// @brief コンストラクタ
        /// @param bus UARTバス
        Ros2UartReader(serial::Serial& bus)
            : serial(bus)
            , buffer(Size)
        {
        }

        /// @brief 更新
        void update()
        {
            if (not serial.isOpen())
            {
                serial.open();
            }
            if (serial.available() >= static_cast<int>(Size))
            {
                std::vector<uint8_t> buf;
                serial.read(buf, Size);
                buffer = std::move(buf);
                serial.flushInput();
            }
        }

        /// @brief メッセージの取得
        Udon::Optional<Message> getMessage() const
        {
            return Udon::Deserialize<Message>(buffer);
        }

        /// @brief メッセージの表示
        void show() const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show("receive failed!");
            }
        }

        /// @brief 受信バッファを表示
        void showRaw() const
        {
            for (int i = 0; i < buffer.size(); i++)
            {
                std::cout << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::endl;
        }
    };
}    // namespace Udon