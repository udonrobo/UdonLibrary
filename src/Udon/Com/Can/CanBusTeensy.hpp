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
//    CAN通信 Teensy用バス管理クラス
//
//-------------------------------------------------------------------

#pragma once

//        vvv teensy3.2 vvv        vvv teensy3.5 vvv         vvv teensy3.6 vvv        vvv teensy4.0/4.1 vvv
#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)

#    include <FlexCAN_T4.h>       // https://github.com/tonton81/FlexCan_T4.git
#    include <IntervalTimer.h>    // https://github.com/loglow/IntervalTimer.git

#    include <Udon/Com/Can/ICanBus.hpp>
#    include <Udon/Com/Can/CanNode.hpp>
#    include <Udon/Com/Can/CanUtility.hpp>
#    include <Udon/Algorithm/StaticVector.hpp>
#    include <Udon/Algorithm/RingBuffer.hpp>

namespace Udon
{

    /// @brief FlexCan_T4ライブラリを用いたCANバスクラス
    /// @tparam Bus バス種類
    /// @remark 使用するTeensyの種類によってCANバスの種類が異なるので注意(対応していないバスを指定するとstatic_assertが発生します)
    /// - Teensy 3.2: CAN0
    /// - Teensy 3.5: CAN0
    /// - Teensy 3.6: CAN0 & CAN1
    /// - Teensy 4.0: CAN1 & CAN2 & CAN3
    template <CAN_DEV_TABLE Bus>
    class CanBusTeensy
        : public ICanBus
    {

        FlexCAN_T4<Bus, RX_SIZE_128, TX_SIZE_256> bus;

        IntervalTimer writeTimer;

        constexpr static uint8_t SingleFrameSize = 8;

        using TxNodePtr = CanNode*;
        struct RxNodePtr
        {
            CanNode* node;
            void (*onReceive)(void*);
            void* p;

            void callback()
            {
                if (onReceive)
                {
                    onReceive(p);
                }
            }
        };

        Udon::StaticVector<TxNodePtr> txNodes;
        Udon::StaticVector<RxNodePtr> rxNodes;

        Udon::RingBuffer<CAN_message_t, 256> txBuffer;

        uint32_t transmitUs = 0;

        static CanBusTeensy* self;    // コールバック関数から自身のインスタンスを参照するためのポインタ (クラステンプレートによってインスタンスごとに別のstatic変数が生成される)

    public:
        /// @brief コンストラクタ
        CanBusTeensy()
        {
            self = this;
        }

        /// @brief コピーコンストラクタ
        CanBusTeensy(const CanBusTeensy&)
        {
            self = this;
        }

        /// @brief デストラクタ
        ~CanBusTeensy()
        {
            end();
        }

        /// @brief 通信開始
        /// @param baudrate 通信レート
        void begin(const uint32_t baudrate = 1000000)
        {
            // バス初期化
            bus.begin();
            bus.setBaudRate(baudrate);

            // 受信開始
            if (rxNodes.size())
            {
                // 受信フィルタ設定 (ノード数が8以下の場合のみ)
                if (rxNodes.size() <= 8)
                {
                    bus.setFIFOFilter(REJECT_ALL);
                    for (size_t i = 0; i < rxNodes.size(); ++i)
                    {
                        bus.setFIFOUserFilter(
                            /* Filter       */ i,
                            /* ID           */ rxNodes[i].node->id,
                            /* Mask         */ 0x7FF,
                            /* Frame type   */ STD,
                            /* Remote frame */ NONE);
                        // 複数のフィルタを設定する関数が用意されているが、適切に受信できていなかったので1つずつ設定している (2023/08/19)
                    }
                }
                else
                {
                    bus.setFIFOFilter(ACCEPT_ALL);
                }

                // FIFOバッファ有効化
                bus.enableFIFO();
                bus.enableFIFOInterrupt();

                // 受信割り込み設定
                bus.onReceive(
                    [](const CAN_message_t& msg)
                    {
                        self->onReceive(msg);
                    });
            }

            // 送信開始
            if (txNodes.size())
            {
                writeTimer.begin(
                    []
                    {
                        if (self->txBuffer.size())
                        {
                            self->bus.write(self->txBuffer.pop());
                        }
                    },
                    200);
            }
        }

        /// @brief 通信終了
        void end()
        {
            bus.disableFIFOInterrupt();
            bus.disableFIFO();
        }

        /// @brief バス更新
        /// @param {transmissionIntervalUs} 送信間隔
        void update(uint32_t transmissionIntervalUs = 5000)
        {
            if (txNodes.size() && micros() - transmitUs >= transmissionIntervalUs)
            {
                onTransmit();
                transmitUs = micros();
            }
        }

        explicit operator bool() const
        {
            return micros() - transmitUs < 100000;
        }

        /// @brief バス情報を表示する
        void show() const
        {
            Serial.print("CanBusTeensy\n");

            Serial.print("\tTX Node\n");
            for (auto&& node : txNodes)
            {
                Serial.printf("\t\tid:%4d   length:%3zu byte", node->id, node->length);
                if (node->length > SingleFrameSize)
                {
                    Serial.print(" (multi frame)");
                }
                else
                {
                    Serial.print(" (single frame)");
                }

                Serial.print("\n\t\t\tdata: ");
                for (size_t i = 0; i < node->length; ++i)
                {
                    Serial.printf("%4d", node->data[i]);
                }

                Serial.println();
            }

            Serial.print("\tRX Node\n");
            for (auto&& rxNode : rxNodes)
            {
                Serial.printf("\t\tid:%4d   size:%3zu byte", rxNode.node->id, rxNode.node->length);
                if (rxNode.node->length > SingleFrameSize)
                {
                    Serial.print(" (multi frame)");
                }
                else
                {
                    Serial.print(" (single frame)");
                }

                Serial.print("\n\t\t\tdata: ");
                for (size_t i = 0; i < rxNode.node->length; ++i)
                {
                    Serial.printf("%4d", rxNode.node->data[i]);
                }

                Serial.println();
            }
        }

        /// @brief 送信ノードをバスに参加させる
        /// @param node 送信ノード
        void joinTx(CanNode& node) override
        {
            txNodes.push_back(&node);
        }

        /// @brief 受信ノードをバスに参加させる
        /// @param node 受信ノード
        void joinRx(CanNode& node, void (*onReceive)(void*), void* p) override
        {
            rxNodes.push_back({ &node, onReceive, p });
        }

        /// @brief 送信ノードをバスから離脱させる
        /// @remark 送信ノードのインスタンスポインタを基に削除します。
        /// @param node 送信ノード
        void leaveTx(const CanNode& node) override
        {
            txNodes.erase(std::find(txNodes.begin(), txNodes.end(), &node));
        }

        /// @brief 受信ノードをバスから離脱させる
        /// @remark 受信ノードのインスタンスポインタを基に削除します。
        /// @param node 受信ノード
        void leaveRx(const CanNode& node) override
        {
            rxNodes.erase(std::find_if(rxNodes.begin(), rxNodes.end(), [&node](const RxNodePtr& rxNode)
                                       { return rxNode.node == &node; }));
        }

    private:
        /// @brief 受信コールバック
        void onReceive(const CAN_message_t& msg)
        {
            //  Q 割り込み時にこのような処理を行うのは良くなこのような
            // A この関数は割り込みコールバック関数なので、できるだけ短い処理を行う必要があります。
            //  この関数内で行っている処理は、受信したメッセージをバッファに格納するだけなので問題ありません。
            // Q いえ、IDの検索を行い、コールバックを呼び出しているので、処理時間が長くなる可能性があるのではないでしょうか？
            
            auto rxNode = std::find_if(rxNodes.begin(), rxNodes.end(), [&msg](const RxNodePtr& rx)
                                       { return rx.node->id == msg.id; });
            if (rxNode == rxNodes.end())
            {
                return;
            }

            Udon::Details::Unpacketize(
                { msg.buf },
                { rxNode->node->data, rxNode->node->length },
                SingleFrameSize);

            if (rxNode->node->length > SingleFrameSize)
            {
                // マルチフレーム
                const size_t frameCount = static_cast<size_t>(std::ceil(
                    static_cast<double>(rxNode->node->length) / SingleFrameSize - 1 /*-1: index*/));

                if (msg.buf[0] == frameCount)
                {
                    rxNode->callback();    // マルチフレームの最終フレームを受信したらコールバックを呼ぶ
                }
            }
            else
            {
                // シングルフレーム
                rxNode->callback();    // シングルフレームの場合は即時コールバックを呼ぶ
            }
        }

        /// @brief 送信処理
        void onTransmit()
        {
            for (auto&& txNode : txNodes)
            {
                CAN_message_t msg;
                msg.id = txNode->id;
                Udon::Details::Packetize(
                    { txNode->data, txNode->length },
                    { msg.buf },
                    SingleFrameSize,
                    [this, &msg](size_t size)
                    {
                        msg.len = SingleFrameSize;
                        // txBuffer.push(msg);
                        while (not bus.write(msg))
                            ;
                        delayMicroseconds(200);
                    });
            }
        }
    };

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;

}    // namespace Udon

#endif
