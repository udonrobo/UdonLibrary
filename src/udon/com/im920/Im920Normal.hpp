//-----------------------------------------------
//
//	UdonLibrary
//
//                2018-2023 Watanabe Rui
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 Fujimoto Ryo
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  通常 IM920
//
//-----------------------------------------------

#pragma once

#include <udon/com/im920/IIm920.hpp>

#include <Arduino.h>

namespace udon
{
    class Im920
        : public IIm920
    {
        HardwareSerial& uart;

        std::vector<uint8_t> receiveBuffer;
        std::vector<uint8_t> sendBuffer;

        uint32_t transmitMs;

    public:
        Im920(HardwareSerial& uart)
            : uart(uart)
            , transmitMs()
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
                receiveUpdate();
                break;
            case TransmitMode::TwoWay:
                twoWayUpdate();
                // todo

                break;
            }
        }

        /// @brief バスの状態を表示
        void show()
        {
            if (operator bool())
            {
                Serial.print("OK ");
            }
            else
            {
                Serial.print("NG ");
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
                // todo
                Serial.print("(TwoWayMode) unsupported! hyahha- ");
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

            uart.print("TXDA");
        }
        void sendUpdate()
        {
            uart.print("TXDA ");
            uint8_t recoveryBuffer = 0;
            uint8_t pos            = 0;
            uint8_t loopCount      = 1;
            for (auto&& it : sendBuffer)
            {
                BitWrite(recoveryBuffer, pos, BitRead(it, 7));
                pos++;

                uart.write(it & 0b01111111);

                if (pos >= 7)
                {
                    if (sendBuffer.size() == loopCount)
                    {
                        // 最後の周回のときどうする？
                        BitWrite(recoveryBuffer, 7, 1);
                    }
                    uart.write(recoveryBuffer);
                    recoveryBuffer = 0;
                    pos            = 0;
                    // 回復ビットを送信したらリセット
                }
                loopCount++;
            }
            // 最後の回復ビットの準備
            if (pos != 0)    // 最後のビットのセットが７ビット目じゃないときだけする
            {
                BitWrite(recoveryBuffer, 7, 1);
                uart.write(recoveryBuffer);
            }
            uart.print("\r\n");
            transmitMs = millis();
        }

        bool receiveUpdate()
        {
            std::vector<uint8_t> lastBuff = receiveBuffer;

            // header = [Node id: 2byte] + [,] + [Transmission module ID: 4byte] + [,] + [RSSI:2byte] + [: ]
            constexpr int HeaderSize = 2 + 1 + 4 + 1 + 2 + 2;

            // footer = [\r] + [\n]
            constexpr int FooterSize = 1 + 1;

            const int frameSize = HeaderSize + static_cast<int>(ceil(receiveBuffer.size() * 1.14)) + FooterSize;

            if (uart.available() >= frameSize)
            {
                // ヘッダを読み飛ばす
                for (size_t i = 0; i < HeaderSize; ++i)
                {
                    (void)uart.read();
                }

                // データ取得
                uint8_t loopCount = 1;    // ループカウンタ
                uint8_t bitCount  = 0;
                for (auto&& it : receiveBuffer)
                {
                    // 復元ビットが含まれないバッファの数

                    // ビット復元処理
                    lastBuff[bitCount] = it = uart.read();
                    bitCount++;

                    uint8_t buf;    // 復元バッファ一時保管データ
                    if (bitCount >= 7)
                    {
                        // 復元バッファの時にビット操作
                        buf = uart.read();
                        for (uint8_t i = 0; i < 7; ++i)
                        {
                            BitWrite(receiveBuffer[loopCount - 1 - 7 + i], 7,
                                     BitRead(buf, i));
                        }
                        bitCount = 0;
                    }
                    else if (receiveBuffer.size() == loopCount)    // 最終データと認識されたとき
                    {
                        buf = uart.read();
                        for (uint8_t i = 0; i < bitCount; ++i)
                        {
                            BitWrite(receiveBuffer[loopCount - 1 - bitCount + i], 7,
                                     BitRead(buf, i));
                        }
                    }
                    if (not BitRead(buf, 7) && receiveBuffer.size() == loopCount)
                    {
                        Serial.print("7bit to 8bit is error!!");
                        Serial.print("\t");
                        receiveBuffer = lastBuff;    // errorが出たときは前周期のデータにする。
                    }
                    loopCount++;
                }

                // フッタを読み飛ばす
                for (size_t i = 0; i < FooterSize; ++i)
                {
                    (void)uart.read();
                }

                // 送信タイミングをリセット
                while (uart.available())
                {
                    (void)uart.read();
                }

                transmitMs = millis();

                return true;
            }
            else
            {
                return false;
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
}    // namespace udon