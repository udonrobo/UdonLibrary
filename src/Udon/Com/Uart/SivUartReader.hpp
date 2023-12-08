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
//    OpenSiv3D用 UART 受信クラス
//
//    Sender --[UART]--> Receiver
//                       ^^^^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include <Siv3D/Serial.hpp>
#include <Udon/Com/Serialization.hpp>
#include <Udon/Common/Show.hpp>

namespace Udon
{
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

        ~SivUartReader()
        {
            isRunning = false;
            thread.join();
        }

        Udon::Optional<Message> getMessage() const
        {
            return Udon::Unpack<Message>(buffer);
        }

        void show(char gap = '\t') const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message, gap);
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
