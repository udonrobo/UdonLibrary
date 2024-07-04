//
//    OpenSiv3D用 UART 受信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//
//    Sender --[UART]--> Receiver
//                       ^^^^^^^^
//

#pragma once

#include <Siv3D/Serial.hpp>
#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{

    /// @brief OpenSiv3D用 UART 受信クラス
    /// @tparam Message メッセージ型
    template <typename Message>
    class SivUartReader
    {
        static constexpr size_t Size = Udon::SerializedSize<Message>();

        s3d::Serial& serial;

        s3d::Array<uint8_t> buffer;

        std::thread thread;

        bool isRunning;    // thread stop token

    public:
        using MessageType = Message;

        /// @brief コンストラクタ
        /// @param bus UARTバス
        SivUartReader(s3d::Serial& bus)
            : serial(bus)
            , buffer(Size)
            , thread(
                  [this]()
                  {
                      while (isRunning)
                      {
                          if (not serial)
                              continue;
                          if (serial.available() < Size)
                              continue;

                          s3d::Array<uint8> temp;
                          if (serial.readBytes(temp) && temp.size() == Size)
                          {
                              buffer = std::move(temp);
                          }

                          serial.clearInput();
                      }
                  })
            , isRunning(true)
        {
        }

        /// @brief デストラクタ
        ~SivUartReader()
        {
            isRunning = false;
            thread.join();
        }

        /// @brief メッセージの取得
        /// @return メッセージ(Optional)
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
                s3d::Print.writeln();
            }
            else
            {
                s3d::Print << U"receive failed!";
            }
        }

        void showRaw() const
        {
            s3d::Print << buffer;
        }
    };
}    // namespace Udon
