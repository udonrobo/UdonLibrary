//-------------------------------------------------------------------
//
//	UdonLibrary
//
//                2018-2023 Watanabe Rui
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 Fujimoto Ryo
//	Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    通常 IM920
//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//    ^^^^^^                                                   ^^^^^^^^
//
//-------------------------------------------------------------------

#pragma once

#include "IIm920.hpp"

#include <Udon/Com/Serialization.hpp>

#include <Arduino.h>

namespace Udon
{
    class Im920
        : public IIm920
    {
        HardwareSerial&      uart;
        std::vector<uint8_t> receiveBuffer;
        std::vector<uint8_t> sendBuffer;
        bool                 twoWayNum;
        uint32_t             sendMitMs;
        uint32_t             receiveDeadTime;

    public:
        Im920(HardwareSerial& uart, bool twoWayNum = false)
            : uart(uart)
            , twoWayNum(twoWayNum)
            , sendMitMs()
            , receiveDeadTime()
        {
        }

        ~Im920() override = default;

        /// @brief IM920が使用可能かどうか
        /// @return IM920が使用可能ならtrue
        operator bool() const override
        {
            return uart && (2 * millis() - (sendMitMs + receiveDeadTime) < 500);
        }

        /// @brief 送信バッファを登録する
        /// @param size 送信バッファのサイズ
        std::vector<uint8_t>& registerSender(size_t size) override
        {
            sendBuffer.resize(size);
            return sendBuffer;
        }

        /// @brief 受信バッファを登録する
        /// @param size 受信バッファのサイズ
        std::vector<uint8_t>& registerReceiver(size_t size) override
        {
            receiveBuffer.resize(size);
            return receiveBuffer;
        }

        /// @brief 通信開始
        /// @param channel チャンネル番号
        void begin(uint8_t channel);

        void update()
        {
            switch (getTransmitMode())
            {
            case TransmitMode::Send:
                sendUpdate();
                break;
            case TransmitMode::Receive:
                receiveUpdate();
                break;
            case TransmitMode::TwoWay:
                twoWayUpdate();
                break;
            }
        }

        /// @brief バスの状態を表示
        void show()
        {
            if (operator bool())
            {
                Serial.print("IM920: OK  ");
            }
            else
            {
                Serial.print("IM920: NG  ");
            }

            switch (getTransmitMode())
            {
            case TransmitMode::Send:
                Serial.print("SendMode ");
                break;
            case TransmitMode::Receive:
                Serial.print("ReceiveMode ");
                break;
            case TransmitMode::TwoWay:
                Serial.print("TwoWayMode ");
                break;
            }
        }

    private:
        enum class TransmitMode
        {
            Send,
            Receive,
            TwoWay,
        };

        /// @brief 通信の状態を取得する
        TransmitMode getTransmitMode() const
        {
            if (sendBuffer.empty())
            {
                return TransmitMode::Receive;
            }
            else if (receiveBuffer.empty())
            {
                return TransmitMode::Send;
            }
            else
            {
                return TransmitMode::TwoWay;
            }
        }

        void sendUpdate()
        {
            uint8_t recoveryBuffer = 0;
            size_t  bitCount       = 0;
            size_t  loopCount      = 0;
            uart.print("TXDA ");

            for (auto&& it : sendBuffer)
            {
                ++loopCount;

                Udon::BitWrite(recoveryBuffer, bitCount, Udon::BitRead(it, 7));
                ++bitCount;

                if (loopCount == 1)
                {
                    uart.write(it | 0b10000000);
                }
                else
                {
                    uart.write(it & 0b01111111);
                }

                if (bitCount >= 7)
                {
                    uart.write(recoveryBuffer);
                    recoveryBuffer = 0;
                    bitCount       = 0;
                }
            }
            if (bitCount != 0)
            {
                uart.write(recoveryBuffer);
            }
            uart.print("\r\n");
            sendMitMs = millis();
        }

        bool receiveUpdate()
        {
            std::vector<uint8_t> newBuffer = receiveBuffer;

            // header = [Node id: 2byte] + [,] + [Transmission module ID: 4byte] + [,] + [RSSI:2byte] + [: ]
            constexpr int HeaderSize = 2 + 1 + 4 + 1 + 2 + 2;

            // footer = [\r] + [\n]
            constexpr int FooterSize = 1 + 1;

            const int dataSize = static_cast<int>(ceil(receiveBuffer.size() * 8.0 / 7.0));

            const int frameSize = HeaderSize + dataSize + FooterSize;

            if (uart.available() >= frameSize)
            {
                size_t  loopCount = 0;
                size_t  bitCount  = 0;
                uint8_t firstBuf;
                do
                {
                    if (uart.available() < dataSize)
                    {
                        return false;
                    }
                    firstBuf = uart.read();
                } while (not(firstBuf & 0b10000000));
                ++bitCount;

                for (auto&& it : newBuffer)
                {
                    ++loopCount;
                    if (loopCount == 1)
                    {
                        it = firstBuf & 0b01111111;
                        continue;
                    }
                    it = uart.read();
                    ++bitCount;
                    if (bitCount >= 7)
                    {
                        uint8_t recoveryBuf = uart.read();
                        for (size_t i = 0; i < 7; ++i)
                        {
                            Udon::BitWrite(newBuffer[loopCount - 7 + i], 7,
                                           Udon::BitRead(recoveryBuf, i));
                        }
                        bitCount = 0;
                    }
                }
                uint8_t lastBuf = uart.read();
                for (size_t i = 0; i < bitCount; ++i)
                {
                    Udon::BitWrite(newBuffer[loopCount - bitCount + i], 7,
                                   Udon::BitRead(lastBuf, i));
                }

                for (size_t i = 0; i < FooterSize; ++i)
                {
                    (void)uart.read();
                }

                if (Udon::CanUnpack(newBuffer))
                {
                    receiveBuffer    = newBuffer;
                    receiveDeadTime = millis();
                }
                return true;
            }
            if (millis() - receiveDeadTime > 1000)
            {    // タイムアウト時エラー吐出
                Serial.print("Im920 is TimeOut!");
                Serial.print("\t");
            }
            return false;
        }

        void twoWayUpdate()
        {
            // transTime =[内部処理時間:10~20ms]+[キャリアセンス:初回5.2ms 連続通信時0.5ms]+[不要データの通信:3.2ms]+[バイトごとの送信時間:0.16ms]
            const double sendTime    = 10.0 + 5.2 + 3.2 + sendBuffer.size() * 0.16;
            const double receiveTime = 10.0 + 5.2 + 3.2 + receiveBuffer.size() * 0.16;

            if (twoWayNum)
            {
                if (2 * millis() - (sendMitMs + receiveDeadTime) > sendTime + receiveTime)
                {    // 時間経過により再送信
                    sendUpdate();
                }
                if (receiveUpdate())
                {
                    sendUpdate();
                }
            }
            else
            {
                if (receiveUpdate())
                {
                    sendUpdate();
                }
            }
        }
    };
    inline void Im920::begin(uint8_t channel)
    {
        // ボーレート設定
        uart.begin(115200);
        // チャンネル設定
        uart.print("STCH ");
        if (channel < 10)
        {
            uart.print("0");    // 0埋め
        }
        uart.print(channel);
        uart.print("\r\n");
        delay(100);

        // 送信出力[10mW]
        uart.print("STPO 3\r\n");
        delay(100);

        // 高速通信モード[50kbps]
        uart.print("STRT 1\r\n");
        delay(100);

        // キャラクタ入出力モード
        uart.print("ECIO\r\n");
        delay(1000);
    }
}    // namespace Udon