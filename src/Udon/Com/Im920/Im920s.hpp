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
//    IM920sL
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
    class Im920s
        : public IIm920
    {
        HardwareSerial&      uart;
        std::vector<uint8_t> receiveBuffer;
        std::vector<uint8_t> sendBuffer;
        uint16_t             nodeNumber;
        uint8_t              busy;
        bool                 twoWayNum;
        uint32_t             sendMitMs;
        uint32_t             receiveDeadTime;

    public:
        Im920s(HardwareSerial& uart, int16_t nodeNumber, uint8_t busyPin, bool twoWayNum = false)
            : uart(uart)
            , nodeNumber(nodeNumber)
            , busy(busyPin)
            , twoWayNum(twoWayNum)
            , sendMitMs()
            , receiveDeadTime()
        {
        }

        ~Im920s() override = default;

        /// @brief IM920が使用可能かどうか
        /// @return IM920が使用可能ならtrue
        operator bool() const override
        {
            switch (getTransmitMode())
            {
            case TransmitMode::Send:
                return uart && (millis() - sendMitMs < 500);
                break;
            case TransmitMode::Receive:
                return uart && (millis() - receiveDeadTime < 500);
                break;
            case TransmitMode::TwoWay:
                return uart && (millis() - ((sendMitMs + receiveDeadTime) / 2) < 500);
                break;
            case TransmitMode::Empty:
                return false;
                break;
            default:
                return false;
                break;
            }
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
            case TransmitMode::Empty:
                break;
            }
        }

        /// @brief バスの状態を表示
        void show()
        {
            if (operator bool())
            {
                Serial.print("IM920s: OK  ");
            }
            else
            {
                Serial.print("IM920s: NG  ");
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
            case TransmitMode::Empty:
                Serial.print("Empty ");
                break;
            }
        }

    private:
        enum class TransmitMode
        {
            Send,
            Receive,
            TwoWay,
            Empty,
        };

        /// @brief 通信の状態を取得する
        TransmitMode getTransmitMode() const
        {
            if (sendBuffer.empty() && !receiveBuffer.empty())
            {
                return TransmitMode::Receive;
            }
            else if (!sendBuffer.empty() && receiveBuffer.empty())
            {
                return TransmitMode::Send;
            }
            else if (!sendBuffer.empty() && !receiveBuffer.empty())
            {
                return TransmitMode::TwoWay;
            }
            else
            {
                return TransmitMode::Empty;
            }
        }

        void sendUpdate()
        {
            uint8_t recoveryBuffer = 0;
            size_t  bitCount       = 0;
            size_t  loopCount      = 0;
            while (digitalRead(busy))
            {
                // busyピンがHIGHの間はコマンドを受け付けない
            }
            // 大量にデータを送らないようにする
            if (millis() - sendMitMs >= 52)
            {
                // uint8_t buffer[100];
                // sprintf((char*)buffer, "TXDU %04d", nodeNumber);
                uart.print("TXDU ");

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
        }

        bool receiveUpdate()
        {
            std::vector<uint8_t> newBuffer = receiveBuffer;

            // header = [Node id: 2byte] + [,] + [Transmission module ID: 4byte] + [,] + [RSSI:2byte] + [: ]
            constexpr int HeaderSize = 2 + 1 + 4 + 1 + 2 + 2;

            // footer = [\r] + [\n]
            constexpr int FooterSize = 1 + 1;

            const int dataSize = static_cast<int>(ceil(receiveBuffer.size() * (8.0 / 7.0)));

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
                    receiveBuffer   = newBuffer;
                    receiveDeadTime = millis();
                }
                return true;
            }
            if (millis() - receiveDeadTime > 1000)
            {    // タイムアウト時エラー吐出
                Serial.print("Im920s is TimeOut!");
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
    inline void Im920s::begin(uint8_t channel)
    {
        // ボーレート設定
        uart.begin(115200);
        // busyピンの設定
        pinMode(busy, INPUT);
        // タイムアウト設定
        uart.setTimeout(1000);
        // 設定書き換えのフラグ
        bool write[] = { false, false, false, false };
        // 設定読み出し
        int    num = 0;
        String str[22];
        char   c[22][80];    // 80は適当
        while (digitalRead(busy))
        {
            // busyピンがHIGHの間はコマンドを受け付けない
        }
        uart.print("RPRM\r\n");
        while (num < 22)
        {
            // タイムアウトが1000msで入っているので自分でdelayを入れなくていい
            str[num++] = uart.readStringUntil('\n');
        }
        // 読み取り確認
        for (int i = 0; i < 22; i++)
            str[i].toCharArray(c[i], 80);
        // チャンネル確認
        int ch = (c[3][5] - '0') * 10 + (c[3][6] - '0');
        if (ch != channel)
            write[0] = true;
        // 送信電力確認
        int power = (c[4][5] - '0');
        if (power != 2)
            write[1] = true;
        // キャラクタ入出力確認
        if (str[13] != "ECIO\r")
            write[2] = true;
        // STATUS出力確認
        if (str[20] != "ERXI\r")
            write[3] = true;
        // 設定書き換え
        if (write[0] || write[1] || write[2] || write[3])
        {
            // 書き込み許可
            while (digitalRead(busy))
            {
                // busyピンがHIGHの間はコマンドを受け付けない
            }
            uart.print("ENWR\r\n");
        }
        if (write[0])
        {
            while (digitalRead(busy))
            {
                // busyピンがHIGHの間はコマンドを受け付けない
            }
            uart.print("STCH ");
            if (channel < 10)
            {
                uart.print("0");
            }
            uart.print(channel);
            uart.print("\r\n");
        }
        if (write[1])
        {
            while (digitalRead(busy))
            {
                // busyピンがHIGHの間はコマンドを受け付けない
            }
            uart.print("STPO 2\r\n");
        }
        if (write[2])
        {
            while (digitalRead(busy))
            {
                // busyピンがHIGHの間はコマンドを受け付けない
            }
            uart.print("ECIO\r\n");
        }
        if (write[3])
        {
            while (digitalRead(busy))
            {
                // busyピンがHIGHの間はコマンドを受け付けない
            }
            uart.print("ERXI\r\n");
        }
        if (write[0] || write[1] || write[2] || write[3])
        {    // 書き込み禁止
            while (digitalRead(busy))
            {
                // busyピンがHIGHの間はコマンドを受け付けない
            }
            uart.print("DSWR\r\n");
        }
        while (digitalRead(busy))
        {
            // busyピンがHIGHの間はコマンドを受け付けない
        }
        // バッファリセット
        while (uart.available() > 0)
            uart.read();
    }
}    // namespace Udon
