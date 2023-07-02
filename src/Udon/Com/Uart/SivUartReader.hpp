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
#include <Udon/Utility/Show.hpp>

namespace Udon
{
    template <typename Message>
    class SivUartReader
    {
        static constexpr size_t Size = Udon::CapacityWithChecksum<Message>();

        s3d::Serial& serial;

        uint8_t buffer[Size];

        std::thread thread;

        bool isRunning;    // thread stop token

    public:
        SivUartReader(s3d::Serial& bus)
            : serial(bus)
            , buffer()
            , thread(
                  [this]()
                  {
                      while (isRunning)
                      {
                          if (not serial)
                              continue;
                          if (serial.available() < Size)
                              continue;

                          Array<uint8> d;
                          if (serial.readBytes(d) && d.size() == Size)
                          {
                              std::copy(d.begin(), d.end(), buffer);
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
                Print.writeln();
            }
            else
            {
                Print << U"receive failed!";
            }
        }

        void showRaw() const
        {
            Print << buffer;
        }
    };
}    // namespace Udon
