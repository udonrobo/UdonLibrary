#pragma once

// 修正
// - ASCIIコード送信 -> バイナリ送信
// - 送受信対応
// - メンバ関数の整理

#include <udon/com/im920/IIm920.hpp>
#include <udon/com/uart/UartBus.hpp>

namespace udon
{
    class Im920
        : public IIm920
    {
        UartBus& uart;

        std::vector<uint8_t> receiveBuffer;
        std::vector<uint8_t> sendBuffer;

        uint32_t transmitMs;

        bool nextTimeSender;

    public:
        Im920(UartBus& uart)
            : uart(uart)
            , transmitMs()
            , nextTimeSender()
        {
        }

        ~Im920() override = default;

        /// @brief IM920が使用可能かどうか
        /// @return IM920が使用可能ならtrue
        operator bool() const override
        {
            return uart && (millis() - transmitMs < 100);
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

                // static byte receiveCount;
                // if (!im920::receiver.flag())
                // {
                //     receiveCount++;
                //     if (receiveCount > 5)
                //     {
                //         receiveCount = 0;
                //         im920::receiver.start();
                //     }
                //     im920::receiver.update();
                // }
                // else
                // {
                //     receiveCount = 0;
                //     im920::sender.update();
                //     im920::receiver.flagreset();
                // }

                // if (millis() - transmitMs > 1000 + random(2000))
                // {
                //     transmitMs = millis();
                //     sendUpdate();
                //     Serial.println("WakeUp");
                // }
                // if (receiveUpdate())
                // {
                //     Serial.println("Receive");
                // }

                if (millis() - transmitMs > 1000 + random(2000))
                {
                    sendUpdate();
                }
                else
                {
                    if (receiveUpdate())
                    {
                        sendUpdate();
                    }
                }
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
            uart.print("TXDA ");
            for (auto&& it : sendBuffer)
            {
                uart.write(it);
            }
            uart.print("\r\n");
            transmitMs = millis();
        }

        bool receiveUpdate()
        {
            // header = [Node id: 2byte] + [,] + [Transmission module ID: 4byte] + [,] + [RSSI:2byte] + [: ]
            constexpr int HeaderSize = 2 + 1 + 4 + 1 + 2 + 2;

            // footer = [\r] + [\n]
            constexpr int FooterSize = 1 + 1;

            const int frameSize = HeaderSize + receiveBuffer.size() + FooterSize;

            if (uart.available() >= frameSize)
            {

                // ヘッダを読み飛ばす
                for (size_t i = 0; i < HeaderSize; ++i)
                {
                    (void)uart.read();
                }

                // データ取得
                for (auto&& it : receiveBuffer)
                {
                    it = uart.read();
                }

                // フッタを読み飛ばす
                for (size_t i = 0; i < FooterSize; ++i)
                {
                    (void)uart.read();
                }

                transmitMs = millis();

                // 送信タイミングをリセット
                while (uart.available())
                {
                    (void)uart.read();
                }

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