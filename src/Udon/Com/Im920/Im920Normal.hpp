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

#include <Udon/Com/Im920/IIm920.hpp>
#include <Udon/Com/Serialization.hpp>

#include <Arduino.h>

namespace Udon
{
    class Im920
        : public IIm920
    {
        HardwareSerial&      uart;
        bool                 twoWayNum;
        std::vector<uint8_t> receiveBuffer;
        std::vector<uint8_t> sendBuffer;

        uint32_t transmitMs;
        bool     NextTrans;
        uint32_t dicConnectCount;

    public:
        Im920(HardwareSerial& uart, bool twoWayNum = false)
            : uart(uart)
            , twoWayNum(twoWayNum)
            , transmitMs()
            , NextTrans()
            , dicConnectCount()
        {
        }

        ~Im920() override = default;

        /// @brief IM920が使用可能かどうか
        /// @return IM920が使用可能ならtrue
        operator bool() const override
        {
            return uart && (millis() - transmitMs < 500);
            return true;
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
                Serial.print(uart.available());
                Serial.print("\t");
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

        void twoWayUpdate()
        {
            // 常時、受信動作を行い、受信、送信を交互に繰り返すように動作させる
            // transTime =[内部処理時間:10~20ms]+[キャリアセンス:初回5.2ms 連続通信時0.5ms]+[不要データの通信:3.2ms]+[バイトごとの送信時間:0.16ms]
            const double sendTime    = 15.0 + 0.5 + 3.2 + sendBuffer.size() * 0.16;
            const double receiveTime = 15.0 + 0.5 + 3.2 + receiveBuffer.size() * 0.16;

            // 受信は二周期に一回
            if (twoWayNum)
            {
                // 発信側
                if (millis() - transmitMs > sendTime + receiveTime)
                {    // 時間経過により再送信
                    sendUpdate();

                    Serial.print("resend");
                    Serial.print("\t");
                }
                else
                {
                    if (receiveUpdate())
                    {
                        // 受け取れたら送り返す
                        sendUpdate();
                    }
                    else
                    {
                        // 受信できなかったらもう一周期待つ
                        Serial.print("buffer didn't charge");
                        Serial.print("\t");
                    }
                }
                // 再送信までの待機時間
            }
            else
            {
                // 受信待ちおよびレスポンス側

                if (receiveUpdate())
                {
                    // 受信出来たら送信モードへ変更
                    sendUpdate();
                }
                else
                {
                    Serial.print("receive miss!");
                    // Serial.print("\t");
                    // 受信できなかったらもう一周期待つ
                }
            }
            // 再送信までの待機時間
        }

        void sendUpdate()
        {
            uint8_t recoveryBuffer = 0;
            uint8_t bitCount       = 0;
            uint8_t loopCount      = 0;
            uart.print("TXDA ");

            for (auto&& it : sendBuffer)
            {
                loopCount++;
                Udon::BitWrite(recoveryBuffer, bitCount, Udon::BitRead(it, 7));
                bitCount++;

                if (loopCount == 1)
                {
                    uart.write(it | 0b10000000);
                    // 最初のビットの８ビット目に１を入れる
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
            transmitMs = millis();
        }

        bool receiveUpdate()
        {
            std::vector<uint8_t> newBuffer = receiveBuffer;

            // header = [Node id: 2byte] + [,] + [Transmission module ID: 4byte] + [,] + [RSSI:2byte] + [: ]
            constexpr int HeaderSize = 2 + 1 + 4 + 1 + 2 + 2;

            // footer = [\r] + [\n]
            constexpr int FooterSize = 1 + 1;

            const int dataSize = static_cast<int>(ceil(receiveBuffer.size() * 1.14));

            const int frameSize = HeaderSize + dataSize + FooterSize;

            if (uart.available() >= frameSize)
            {
                uint8_t loopCount = 0;
                uint8_t bitCount  = 0;
                uint8_t firstBit;
                do
                {
                    if (uart.available() < frameSize)
                    {
                        ++dicConnectCount;
                        return true;
                    }
                    firstBit = uart.read();
                } while (not(firstBit & 0b10000000));
                ++bitCount;

                newBuffer[0] = firstBit & 0b01111111;

                for (auto&& it : newBuffer)
                {
                    ++loopCount;
                    if (loopCount == 1)
                    {
                        continue;
                    }
                    it = uart.read();
                    ++bitCount;
                    if (bitCount >= 7)
                    {
                        uint8_t buf = uart.read();
                        for (uint8_t i = 0; i < 7; ++i)
                        {
                            Udon::BitWrite(newBuffer[loopCount - 7 + i], 7,
                                           Udon::BitRead(buf, i));
                        }
                        bitCount = 0;
                    }
                }

                if (newBuffer.size() == loopCount)
                {
                    uint8_t buf = uart.read();
                    for (uint8_t i = 0; i < bitCount; ++i)
                    {
                        Udon::BitWrite(newBuffer[loopCount - bitCount + i], 7,
                                       Udon::BitRead(buf, i));
                    }
                }
                for (size_t i = 0; i < FooterSize; ++i)
                {
                    (void)uart.read();
                }

                Deserializer deserializer(newBuffer);
                if (deserializer)
                {
                    receiveBuffer   = newBuffer;
                    transmitMs      = millis();
                    dicConnectCount = 0;
                }
                else
                {
                    ++dicConnectCount;
                    // Serial.print("data is not correct!");
                }
            }
            else
            {
                ++dicConnectCount;
            }

            if (dicConnectCount > 20)
            {
                while (uart.available())
                {
                    (void)uart.read();
                }
                receiveBuffer.clear();
                Serial.print("bit did not charged!");
                return false;
            }
            else
            {
                return true;
            }
        }
    };

    // detail

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