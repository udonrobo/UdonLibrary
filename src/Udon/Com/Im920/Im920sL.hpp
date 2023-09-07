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
#include <Udon/Utility/SerialPrintf.hpp>

namespace Udon
{
    class Im920sL
        : public IIm920
    {
        HardwareSerial& uart;

        Im920Node* txNode;
        Im920Node* rxNode;

        Udon::Optional<uint16_t> nodeNum;
        Udon::Optional<uint8_t>  busyPin;

        uint32_t lastWaitUntilCommandAcceptMs = 0;
        uint32_t lastRestartMs                = 0;

    public:
        /// @brief 送信者用コンストラクタ
        /// @param uart IM920と接続されたシリアルポート
        /// @param nodeNum 相手方のノード番号
        /// @param busyPin busyピンのピン番号
        Im920sL(HardwareSerial& uart, uint16_t nodeNum, uint8_t busyPin)
            : uart(uart)
            , txNode()
            , rxNode()
            , nodeNum(nodeNum)
            , busyPin(busyPin)
        {
        }

        /// @brief 送信者用コンストラクタ
        /// @param uart IM920と接続されたシリアルポート
        /// @param nodeNum 相手方のノード番号
        Im920sL(HardwareSerial& uart, uint16_t nodeNum)
            : uart(uart)
            , txNode()
            , rxNode()
            , nodeNum(nodeNum)
            , busyPin()
        {
        }

        /// @brief 受信者用コンストラクタ
        /// @param uart IM920と接続されたシリアルポート
        /// @remark このコンストラクタを呼び出すのは受信者のみ存在する場合です。送信者が存在する場合は送信先のノード番号を指定する必要があります。
        Im920sL(HardwareSerial& uart)
            : uart(uart)
            , txNode()
            , rxNode()
            , nodeNum()
            , busyPin()
        {
        }

        /// @brief IM920の有効性を取得
        /// @return 有効ならtrue
        operator bool() const override { return not isTimeout(700); }

        /// @brief 通信開始
        /// @param channel チャンネル番号
        bool begin(uint8_t channel);

        /// @brief 通信更新
        void update();

        /// @brief IM920の状態を表示
        void show() const
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

        /// @brief IM920sLで使用可能なチャンネル数に制限をかける
        static int ClampChannel(int channel)
        {
            return constrain(channel, 1, 45);
        }

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

        bool sendUpdate()
        {
            if (not nodeNum)
            {
                return false;
            }

            const int sendTimeMs = 52;  // 連続で送信する場合、52ms送信休止時間が必要 (説明書 ７－３（２）送信休止時間 参照)

            if (millis() - txNode->transmitMs < sendTimeMs)
            {
                return false;
            }
            
            // データ送信
            {
                Udon::SerialPrintf(uart, "TXDU %04d ", *nodeNum);

                Udon::BitPack(txNode->data, txNode->data + txNode->size, [this](uint8_t data)
                            { uart.write(data); });

                uart.print("\r\n");
            }

            if (uart.readStringUntil('\n') == "OK\r")
            {
                txNode->transmitMs = millis();
            }

            return true;
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
            // const double sendTime    = 10.0 + 5.2 + 3.2 + txNode->size * 0.16;
            // const double receiveTime = 10.0 + 5.2 + 3.2 + rxNode->size * 0.16;

            // static uint32_t lastTransmitMs = 0;

            // if (millis() - lastTransmitMs > sendTime + receiveTime && receiveUpdate())
            // {
            //     sendUpdate();
            //         lastTransmitMs = millis();
            // }
            // else
            // {
            //     if (millis() - lastTransmitMs > random(2000))
            //     {
            //         sendUpdate();

            //         lastTransmitMs = millis();
            //     }
            // }
        }

        bool isTimeout(uint32_t timeoutMs) const
        {
            switch (getTransmitMode())
            {
            case TransmitMode::Send:
                return millis() - txNode->transmitMs > timeoutMs;
            case TransmitMode::Receive:
                return millis() - rxNode->transmitMs > timeoutMs;
            case TransmitMode::TwoWay:
                return millis() - txNode->transmitMs > timeoutMs || millis() - rxNode->transmitMs > timeoutMs;
            case TransmitMode::Empty:
                return false;
            default:
                return false;
            }
        }

        void RestartOnTimeout()
        {
            if (not isTimeout(2000))
            {
                return;
            }

            // 通信が不通になった場合はソフトウエアリセット
            if (millis() - lastRestartMs > 2000)
            {
                waitUntilCommandAccept();
                uart.print("SRST\r\n");
                lastRestartMs = millis();
                while (uart.available())
                {
                    uart.read();
                }
            }
        }

        /// @brief IM920がコマンドを受け付けるまで待つ
        void waitUntilCommandAccept()
        {
            if (busyPin)
            {
                lastWaitUntilCommandAcceptMs = millis();

                while (digitalRead(*busyPin))    // busyピンがHIGHの間はコマンドを受け付けない
                {
                    delayMicroseconds(10);    // チャタリング防止

                    if (millis() - lastWaitUntilCommandAcceptMs > 200)
                    {
                        break;  // タイムアウトした場合は強制的にコマンドを受け付ける
                    }
                }
            }
            else
            {
                delay(200);    // busy pinが設定されていない場合は200ms待つ
            }
        }
    };

    inline bool Im920sL::begin(uint8_t channel)
    {
        // ボーレート設定
        uart.begin(115200);

        while (uart.available())
        {
            uart.read();
        }        

        if (busyPin)
        {
            pinMode(*busyPin, INPUT);
        }

        // タイムアウト設定
        uart.setTimeout(10);

        // パラメーター一括読み出し
        waitUntilCommandAccept();
        uart.print("RPRM\r\n");

        (void)uart.readStringUntil('\n');                                              //  ID:
        (void)uart.readStringUntil('\n');                                              //  STNN:
        (void)uart.readStringUntil('\n');                                              //  STGN:
        (void)uart.readStringUntil('\n');                                              //  STRT:
        const int defaultChannel = uart.readStringUntil('\n').substring(5).toInt();    //  STCH:
        const int defaultPower   = uart.readStringUntil('\n').substring(5).toInt();    //  STPO:
        (void)uart.readStringUntil('\n');                                              //  STNM:
        (void)uart.readStringUntil('\n');                                              //  STTH:
        (void)uart.readStringUntil('\n');                                              //  SRTH:
        (void)uart.readStringUntil('\n');                                              //  WTRT:
        (void)uart.readStringUntil('\n');                                              //  STTL:
        (void)uart.readStringUntil('\n');                                              //  STTR:
        (void)uart.readStringUntil('\n');                                              //  SSTM:
        (void)uart.readStringUntil('\n');                                              //  SWTM:
        (void)uart.readStringUntil('\n');                                              //  STTN:
        (void)uart.readStringUntil('\n');                                              //  STAT:
        (void)uart.readStringUntil('\n');                                              //  STTG:
        (void)uart.readStringUntil('\n');                                              // [ENRX]   DSRX   スリープ
        (void)uart.readStringUntil('\n');                                              // [ENAK]
        const String defaultCharIOMode = uart.readStringUntil('\n');                   // [ECIO]   DCIO   キャラクタ入出力モード
        (void)uart.readStringUntil('\n');                                              //  EENC   [DENC]  パケット暗号化
        (void)uart.readStringUntil('\n');                                              //  ENAD   [DSAD]  AD 入力モード
        (void)uart.readStringUntil('\n');                                              // [DSAR]   ENAR   リトライ送信
        (void)uart.readStringUntil('\n');                                              //  ENSS   [DSSS]  同期スリープ開始・解除
        (void)uart.readStringUntil('\n');                                              //  ESNF   [DSNF]  スニファモード
        (void)uart.readStringUntil('\n');                                              //  ENRC   [DSRC]  リモートコマンド受信
        (void)uart.readStringUntil('\n');                                              //  ETRC   [DTRC]  リモートコマンド送信
        (void)uart.readStringUntil('\n');                                              //  DORT
        (void)uart.readStringUntil('\n');                                              //  DADR
        (void)uart.readStringUntil('\n');                                              //  DOMI
        (void)uart.readStringUntil('\n');                                              //  ENHP
        (void)uart.readStringUntil('\n');                                              //  DSEF
        const String defaultStatusOutputOnReceipt = uart.readStringUntil('\n');        // [ERXI]   DRXI    受信時のステータス出力
        (void)uart.readStringUntil('\n');                                              //  ENWR   [DSWR]   フラッシュメモリ書き込み許可

        // フラッシュメモリ書き込み許可
        waitUntilCommandAccept();
        uart.print("ENWR\r\n");
        if (uart.readStringUntil('\n') != "OK\r")
        {
            return false;
        }

        // チャンネル設定
        if (channel != defaultChannel)
        {
            waitUntilCommandAccept();
            Udon::SerialPrintf(uart, "STCH %02d\r\n", channel);
            if (uart.readStringUntil('\n') != "OK\r")
            {
                return false;
            }
        }

        // 出力電力設定
        if (defaultPower != 2)
        {
            waitUntilCommandAccept();
            Udon::SerialPrintf(uart, "STPO %d\r\n", 2);
            if (uart.readStringUntil('\n') != "OK\r")
            {
                return false;
            }
        }

        // キャラクタ入出力モード設定
        if (defaultCharIOMode != "ECIO\r")
        {
            waitUntilCommandAccept();
            uart.print("ECIO\r\n");
            if (uart.readStringUntil('\n') != "OK\r")
            {
                return false;
            }
        }

        // 受信時のステータス出力設定
        if (defaultStatusOutputOnReceipt != "ERXI\r")
        {
            waitUntilCommandAccept();
            uart.print("ERXI\r\n");
            if (uart.readStringUntil('\n') != "OK\r")
            {
                return false;
            }
        }

        // フラッシュメモリ書き込み禁止
        waitUntilCommandAccept();
        uart.print("DSWR\r\n");
        if (uart.readStringUntil('\n') != "OK\r")
        {
            return false;
        }

        return true;
    }

    inline void Im920sL::update()
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

        RestartOnTimeout();
    }

}    // namespace Udon
