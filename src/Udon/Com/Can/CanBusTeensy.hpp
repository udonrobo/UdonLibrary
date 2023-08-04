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

    /// @brief FlexCan_T4ライブラリを用いたCANバス管理クラス
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

        IntervalTimer readTimer;
        IntervalTimer writeTimer;

        constexpr static uint32_t SingleFrameSize = 8;

        using TxNode = CanNode;
        struct RxNode
        {
            CanNode* node;
            void (*onReceive)(void*);
            void* p;
        };

        Udon::StaticVector<TxNode*> txNodes;
        Udon::StaticVector<RxNode>  rxNodes;

        Udon::RingBuffer<CAN_message_t, 256> txBuffer;

        uint32_t transmitUs = 0;

        static CanBusTeensy* self;

    public:
        /// @brief コンストラクタ
        CanBusTeensy()
        {
            self = this;
        }

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
            if (rxNodes.size() || txNodes.size())
            {
                bus.begin();
                bus.setBaudRate(baudrate);
                bus.enableFIFO();
                bus.enableFIFOInterrupt();
            }
            if (rxNodes.size())
            {
                bus.onReceive(onReceive);
                readTimer.begin(
                    []
                    {
                        self->bus.events();
                    },
                    200);
            }
            if (txNodes.size())
            {
                writeTimer.begin(
                    []
                    {
                        if (self->txBuffer.size())
                        {
                            Serial.print(self->txBuffer.size());
                            Serial.print(" ");
                            auto o = self->txBuffer.pop();
                            Serial.print(o.id);
                                Serial.print(o.buf[0], HEX);
                            Serial.println();
                            while (not self->bus.write(o))
                                ;
                        }
                    },
                    1000);
            }
        }

        /// @brief 通信終了
        void end()
        {
            bus.disableFIFO();
            bus.disableFIFOInterrupt();
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

        void show() const
        {
            Serial.print("Bus: CAN 2.0B\n");

            Serial.print("\tTX Node\n");
            for (auto&& node : txNodes)
            {
                Serial.printf("\t\tid:%4d   length:%3zu byte", node->id, node->length);
                if (node->length > SingleFrameSize)
                {
                    Serial.print(" (multi packet)");
                }
                else
                {
                    Serial.print(" (single packet)");
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
                    Serial.print(" (multi packet)");
                }
                else
                {
                    Serial.print(" (single packet)");
                }

                Serial.print("\n\t\t\tdata: ");
                for (size_t i = 0; i < rxNode.node->length; ++i)
                {
                    Serial.printf("%4d", rxNode.node->data[i]);
                }

                Serial.println();
            }
        }

        void joinTx(CanNode& node) override
        {
            txNodes.push_back(&node);
        }

        void joinRx(CanNode& node, void (*onReceive)(void*), void* p) override
        {
            rxNodes.push_back({ &node, onReceive, p });
        }

        void leaveTx(CanNode& node) override
        {
            txNodes.erase(std::find(txNodes.begin(), txNodes.end(), &node));
        }

        void leaveRx(CanNode& node) override
        {
            rxNodes.erase(std::find_if(rxNodes.begin(), rxNodes.end(), [&node](const RxNode& rxNode)
                                       { return rxNode.node == &node; }));
        }

    private:
        /// @brief 受信コールバック
        static void onReceive(const CAN_message_t& msg)
        {
            auto rxNode = std::find_if(self->rxNodes.begin(), self->rxNodes.end(), [&msg](const RxNode& rx)
                                       { return rx.node->id == msg.id; });
            if (rxNode == self->rxNodes.end())
            {
                return;
            }

            Udon::Unpacketize(
                { const_cast<CAN_message_t&>(msg).buf },
                { rxNode->node->data, rxNode->node->length },
                SingleFrameSize);

            const size_t packetCount = static_cast<size_t>(std::ceil(
                static_cast<double>(rxNode->node->length) / SingleFrameSize - 1 /*-1: index*/));

            if (rxNode->node->length > SingleFrameSize)
            {
                if (msg.buf[0] == packetCount)
                {
                    // マルチフレームの最終フレームを受信したらコールバックを呼ぶ
                    if (rxNode->onReceive)
                    {
                        rxNode->onReceive(rxNode->p);
                    }
                }
            }
            else
            {
                // シングルフレームの場合は即時コールバックを呼ぶ
                if (rxNode->onReceive)
                {
                    rxNode->onReceive(rxNode->p);
                }
            }
        }

        /// @brief 送信処理
        void onTransmit()
        {
            for (auto&& txNode : txNodes)
            {
                CAN_message_t msg{};
                msg.id = txNode->id;
                Udon::Packetize(
                    { txNode->data, txNode->length },
                    { msg.buf },
                    SingleFrameSize,
                    [this, &msg](size_t size)
                    {
                        msg.len = SingleFrameSize;
                        txBuffer.push(msg);
                    });
            }
        }
    };

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;

}    // namespace Udon

#endif
