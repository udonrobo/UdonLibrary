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
            return uart /*&& (millis() - transmitMs < 1000)*/;
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

            uart.print("TXDA ");
        }
        void sendUpdate()
        {
            uart.print("TXDA ");
            uint8_t recoveryBuffer = 0;
            uint8_t pos            = 0;
            uint8_t loopCount      = 1;

            for (auto&& it : sendBuffer)
            {
                udon::BitWrite(recoveryBuffer, pos, udon::BitRead(it, 7));
                pos++;

                if (loopCount == 1)
                {
                    udon::BitWrite(it, 7, 1);
                    uart.write(it);
                }
                else
                {
                    uart.write(it & 0b01111111);
                }    // 最初のビットの８ビット目に１を入れる

                if (pos >= 7)
                {
                    uart.write(recoveryBuffer);

                    recoveryBuffer = 0;
                    pos            = 0;
                    // 回復ビットを送信したらリセット
                }
                loopCount++;
            }
            if (pos != 0)
            {
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
            // const int frameSize = HeaderSize + receiveBuffer.size() + FooterSize;

            Serial.print(uart.available());
            Serial.print("\t");

            if (uart.available() >= frameSize)
            {    // ヘッダの内容によって受信対応を決める

                // // ヘッダを読み飛ばす
                // for (size_t i = 0; i < HeaderSize; ++i)
                // {
                //     if (i < 2)
                //     {
                //         uint8_t buf = uart.read();
                //         Serial.print((char)buf);
                //         if ( buf !=48)
                //         {
                //             Serial.print("Data different!");
                //             while (uart.available())
                //             {
                //                 (void)uart.read();
                //             }
                //             transmitMs = millis();
                //             return true;
                //         }
                //     }
                //     else
                //     {
                //         Serial.print((char) uart.read());
                //     }
                // }

                uint8_t loopCount = 0;    // ループカウンタ
                uint8_t bitCount  = 0;    // ビットカウンタ
                uint8_t pribuff   = 0;

                do
                {
                    pribuff = uart.read();
                } while (not(pribuff & 0b10000000));
                // if (uart.available() <= frameSize - (HeaderSize + FooterSize) - 1)
                // {
                //     Serial.print("a");
                // }
                bitCount++;
                // 最初のビットであることを確認してから次の動作を実行する
                receiveBuffer[0] = pribuff & 0b01111111;

                // 合わない時はスキップ
                // Serial.print("7bit to 8bit!!");
                // Serial.print("\t");
                // receiveBufferを触らずループを抜ける

                // forを一回スキップ

                // // データ取得
                // uint8_t loopCount = 0;    // ループカウンタ
                // uint8_t bitCount  = 0;    // ビットカウンタ
                for (auto&& it : receiveBuffer)
                {
                    loopCount++;
                    if (loopCount == 1)
                    {
                        continue;
                    }

                    // if (loopCount == 1)
                    // {    // 最初の周期のみ実行したい
                    //     uint8_t pribuff;
                    //     pribuff = uart.read();
                    //     bitCount++;

                    //     if (udon::BitRead(pribuff, 7))
                    //     {
                    //         // 最初のビットであることを確認してから次の動作を実行する
                    //         it = pribuff & 0b01111111;
                    //     }
                    //     else
                    //     {
                    //         // 合わない時はスキップ
                    //         Serial.print("7bit to 8bit is error!!");
                    //         Serial.print("\t");
                    //         break;    // receiveBufferを触らずループを抜ける
                    //     }
                    //     continue;    // forを一回スキップ
                    // }

                    // ビット復元処理
                    it = uart.read();
                    bitCount++;
                    if (bitCount >= 7)
                    {
                        // 復元バッファの時にビット操作
                        uint8_t buf = uart.read();
                        for (uint8_t i = 0; i < 7; ++i)
                        {
                            udon::BitWrite(receiveBuffer[static_cast<std::vector<uint8_t, std::allocator<uint8_t>>::size_type>(loopCount) - 7 + i], 7,
                                           udon::BitRead(buf, i));
                        }
                        bitCount = 0;
                    }
                }

                if (receiveBuffer.size() == loopCount)    // 最終データと認識されたとき
                {
                    uint8_t buf = uart.read();
                    for (uint8_t i = 0; i < bitCount; ++i)
                    {
                        udon::BitWrite(receiveBuffer[static_cast<std::vector<uint8_t, std::allocator<uint8_t>>::size_type>(loopCount) - bitCount + i], 7,
                                       udon::BitRead(buf, i));
                    }
                }

                // // フッタを読み飛ばす
                // for (size_t i = 0; i < FooterSize; ++i)
                // {
                //     (void)uart.read();
                // }

                // //送信タイミングをリセット

                while (uart.available() > frameSize*3)
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