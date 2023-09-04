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

#include <Udon/Algorithm/BitPack.hpp>
#include <Udon/Stl/Optional.hpp>

namespace Udon
{
    class Im920sL
        : public IIm920
    {
        HardwareSerial& uart;

        Im920Node* txNode;
        Im920Node* rxNode;

        uint16_t nodeNum;

        Udon::Optional<uint8_t> busyPin;

    public:
        /// @brief コンストラクタ
        /// @param uart IM920と接続されたシリアルポート
        /// @param nodeNum 相手方のノード番号
        /// @param busyPin busyピンのピン番号
        Im920sL(HardwareSerial& uart, uint16_t nodeNum, uint8_t busyPin)
            : uart(uart)
            , nodeNum(nodeNum)
            , busyPin(busyPin)
        {
        }

        /// @brief コンストラクタ
        /// @param uart IM920と接続されたシリアルポート
        /// @param nodeNum 相手方のノード番号
        Im920sL(HardwareSerial& uart, uint16_t nodeNum)
            : uart(uart)
            , nodeNum(nodeNum)
            , busyPin(Udon::nullopt)
        {
        }

        /// @brief IM920が使用可能かどうか
        /// @return IM920が使用可能ならtrue
        operator bool() const override
        {
            switch (getTransmitMode())
            {
            case TransmitMode::Send:
                return uart && (millis() - txNode->transmitMs < 500);
                break;
            case TransmitMode::Receive:
                return uart && (millis() - rxNode->transmitMs < 500);
                break;
            case TransmitMode::TwoWay:
                return uart && (millis() - ((txNode->transmitMs + rxNode->transmitMs) / 2) < 500);
                break;
            case TransmitMode::Empty:
                return false;
                break;
            default:
                return false;
                break;
            }
        }

        /// @brief 通信開始
        /// @param channel チャンネル番号
        void begin(uint8_t channel);

        void update()
        {
            switch (getTransmitMode())
            {
            case TransmitMode::Send: sendUpdate(); break;
            case TransmitMode::Receive: receiveUpdate(); break;
            case TransmitMode::TwoWay: twoWayUpdate(); break;
            case TransmitMode::Empty: break;
            }
        }

        /// @brief バスの状態を表示
        void show()
        {
            if (operator bool())
            {
                Serial.print("IM920sL: OK  ");
            }
            else
            {
                Serial.print("IM920sL: NG  ");
            }

            switch (getTransmitMode())
            {
            case TransmitMode::Send: Serial.print("SendMode "); break;
            case TransmitMode::Receive: Serial.print("ReceiveMode "); break;
            case TransmitMode::TwoWay: Serial.print("TwoWayMode "); break;
            case TransmitMode::Empty: Serial.print("Empty "); break;
            }
        }

        /// @brief 自身のノード番号を表示
        /// @remark 相手方のIM920クラスコンストラクタの引数に渡すときに使うと便利です。
        /// @return 自身のノード番号
        void showNodeNumber()
        {
            waitUntilCommandAccept();
            uart.print("RDNN\r\n");
            Serial.print(uart.readStringUntil('\n'));
        }

        /// @brief 送信ノードを登録
        /// @param node
        void joinTx(Im920Node& node) override { txNode = &node; }

        /// @brief 受信ノードを登録
        void joinRx(Im920Node& node) override { rxNode = &node; }

        /// @brief 送信ノードを登録解除
        void leaveTx() override { txNode = nullptr; }

        /// @brief 受信ノードを登録解除
        void leaveRx() override { rxNode = nullptr; }

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
            uart.print("TXDU ");
            uart.print("0001");
            Udon::BitPack(txNode->data, txNode->data + txNode->size, [&](uint8_t data)
                          { uart.write(data); });
            uart.print("\r\n");
            if (uart.readStringUntil('\n') == "OK\r")
            {
                txNode->transmitMs = millis();
            }
            Serial.println(txNode->transmitMs);
        }

        bool receiveUpdate()
        {
            return false;
        }

        void twoWayUpdate()
        {
            // transTime =[内部処理時間:10~20ms]+[キャリアセンス:初回5.2ms 連続通信時0.5ms]+[不要データの通信:3.2ms]+[バイトごとの送信時間:0.16ms]
            const double sendTime    = 10.0 + 5.2 + 3.2 + (txNode ? 1 : 0) * 0.16;
            const double receiveTime = 10.0 + 5.2 + 3.2 + (rxNode ? 1 : 0) * 0.16;

            if (false)
            {
                if (2 * millis() - (txNode->transmitMs + rxNode->transmitMs) > sendTime + receiveTime)
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

        void waitUntilCommandAccept() const
        {
            if (busyPin)
            {
                while (digitalRead(*busyPin))    // busyピンがHIGHの間はコマンドを受け付けない
                    ;
            }
            else
            {
                delay(200);    // busy pinが設定されていない場合は200ms待つ
            }
        }
    };

    inline void Im920sL::begin(uint8_t channel)
    {
        // ボーレート設定
        uart.begin(115200);

        // busyピン設定
        if (busyPin)
            pinMode(*busyPin, INPUT);

        // タイムアウト設定
        uart.setTimeout(100);

        // フラッシュメモリ書き込み許可
        {
            waitUntilCommandAccept();
            uart.print("ENWR\r\n");
            uart.readStringUntil('\n');
        }

        // パラメーター一括読み出し
        {
            waitUntilCommandAccept();
            uart.print("RPRM\r\n");

            for (int i = 0; i < 4; ++i)
            {
                uart.readStringUntil('\n');
            }

            const auto channel = uart.readStringUntil('\n');
            const auto power   = uart.readStringUntil('\n');
            Serial.println(channel);    // STCH 01
            Serial.println(power);      // STPO 2

            for (int i = 0; i < 13; ++i)
            {
                uart.readStringUntil('\n');
            }

            const auto ecio = uart.readStringUntil('\n');
            Serial.println(ecio);    // ECIO

            for (int i = 0; i < 12; ++i)
            {
                uart.readStringUntil('\n');
            }

            const auto erxi = uart.readStringUntil('\n');
            Serial.println(erxi);    // ERXI
        }
    }
}    // namespace Udon
