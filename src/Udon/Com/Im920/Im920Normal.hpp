//
//    通常 IM920
//
//                  2018-2023 Watanabe Rui
//	  Copyright (c) 2022-2023 Okawa Yusuke
//	  Copyright (c) 2022-2023 Fujimoto Ryo
//	  Copyright (c) 2022-2023 udonrobo
//

//
//    Sender --[UART]--> IM920 ~~[920MHz]~~> IM920 --[UART]--> Receiver
//    ^^^^^^                                                   ^^^^^^^^
//

#pragma once

#include "IIm920.hpp"

#include <Udon/Algorithm/BitPack.hpp>
#include <Udon/Types/Optional.hpp>
#include <Udon/Common/Printf.hpp>
#include <Udon/Types/ArrayView.hpp>

namespace Udon
{
    class Im920
        : public IIm920
    {
        HardwareSerial& uart;

        Im920Node* txNode;
        Im920Node* rxNode;

        bool     twoWayNum;
        uint32_t sendMitMs;
        uint32_t receiveDeadTime;

    public:
        Im920(HardwareSerial& uart, bool twoWayNum = false)
            : uart(uart)
            , twoWayNum(twoWayNum)
            , sendMitMs()
            , receiveDeadTime()
        {
        }

        /// @brief IM920が使用可能かどうか
        /// @return IM920が使用可能ならtrue
        operator bool() const override
        {
            switch (getTransmitMode())
            {
            case TransmitMode::Send:
                return uart && (millis() - sendMitMs < 1000);
            case TransmitMode::Receive:
                return uart && (millis() - receiveDeadTime < 1000);
            case TransmitMode::TwoWay:
                return uart && (2 * millis() - (sendMitMs + receiveDeadTime) < 1000);
            default:
                return false;
            }
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
            default:
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
            case TransmitMode::Empty:
                Serial.print("Empty ");
                break;
            }
        }

        /// @brief 送信ノードを登録
        /// @param node
        void joinTx(Im920Node& node) override { txNode = &node; }

        /// @brief 受信ノードを登録
        void joinRx(Im920Node& node) override { rxNode = &node; }

    private:
        enum class TransmitMode
        {
            Send,
            Receive,
            TwoWay,
            Empty,
        };

        /// @brief 通信モードを取得する
        TransmitMode getTransmitMode() const
        {
            if (txNode)
            {
                if (rxNode)
                {
                    return TransmitMode::TwoWay;
                }
                else
                {
                    return TransmitMode::Send;
                }
            }
            else if (rxNode)
            {
                return TransmitMode::Receive;
            }
            else
            {
                return TransmitMode::Empty;
            }
        }

        void sendUpdate()
        {
            uart.print("TXDA ");

            Udon::BitPack(txNode->data, txNode->data + txNode->size, [this](uint8_t data)
                          { uart.write(data); });

            uart.print("\r\n");
        }

        bool receiveUpdate()
        {
            // header = [Dummy: 2byte] + [,] + [Node number: 4byte] + [,] + [RSSI: 2byte] + [:]
            constexpr int HeaderSize = 2 + 1 + 4 + 1 + 2 + 1;

            // footer = [\r] + [\n]
            constexpr int FooterSize = 1 + 1;

            // FrameSize = header + data + footer
            const int FrameSize = HeaderSize + Udon::BitPackedSize(rxNode->size) + FooterSize;

            // 受信バッファにデータがない場合は何もしない
            if (uart.available() < FrameSize)
            {
                return false;
            }

            // ヘッダー読み捨て
            for (int i = 0; i < HeaderSize; ++i)
            {
                uart.read();
            }

            // データ読み込み
            if (not Udon::BitUnpack(rxNode->data, rxNode->data + rxNode->size, [this]() -> uint8_t
                                    { return uart.read(); }))
            {
                // 先頭バイトの  MSB が 1 でない場合はデータが壊れているので読み捨て
                while (uart.available())
                {
                    uart.read();
                }
                return false;
            }

            // フッター読み捨て
            for (int i = 0; i < FooterSize; ++i)
            {
                uart.read();
            }

            // バッファに残っているデータを読み捨て
            while (uart.available())
            {
                uart.read();
            }

            rxNode->transmitMs = millis();

            return true;
        }

        void twoWayUpdate()
        {
            // transTime =[内部処理時間:10~20ms]+[キャリアセンス:初回5.2ms 連続通信時0.5ms]+[不要データの通信:3.2ms]+[バイトごとの送信時間:0.16ms]
            const double sendTime    = 10.0 + 5.2 + 3.2 + txNode->size * 0.16;
            const double receiveTime = 10.0 + 5.2 + 3.2 + rxNode->size * 0.16;

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
        Udon::Printf(uart, "STCH %02d\r\n", channel);
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