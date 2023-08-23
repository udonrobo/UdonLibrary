#pragma once

#if defined(ARDUINO_ARCH_RP2040)

#    include <Udon/Com/Can/ICanBus.hpp>
#    include <Udon/Com/Can/CanNode.hpp>
#    include <Udon/Com/Can/CanUtility.hpp>

#    include <Udon/Algorithm/StaticVector.hpp>
#    include <Udon/Algorithm/RingBuffer.hpp>

#    include <Udon/Pio/PicoPioClock.hpp>

#    include <Udon/Thirdparty/PicoMcp2515/mcp2515.h>

namespace Udon
{
    class CanBusPico
        : public ICanBus
    {
        MCP2515 bus;

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

        Udon::RingBuffer<can_frame, 256> txBuffer;

        uint32_t timestampUs = 0;

        uint8_t intPin;

    public:
        CanBusPico(
            uint8_t  intPin,
            uint8_t  mosiPin,
            uint8_t  misoPin,
            uint8_t  sckPin,
            uint8_t  csPin,
            uint32_t spiClock = 1000000)
            : bus(csPin, mosiPin, misoPin, sckPin, spiClock)
            , intPin(intPin)
        {
        }

        void begin(CAN_CLOCK transceiverClock = MCP_16MHZ, CAN_SPEED canSpeed = CAN_1000KBPS)
        {
            SPI.begin();
            bus.reset();
            bus.setBitrate(canSpeed, transceiverClock);
            bus.setNormalMode();
            pinMode(intPin, INPUT_PULLUP);
            attachInterruptParam(
                digitalPinToInterrupt(intPin),
                [](void* self)
                {
                    static_cast<CanBusPico*>(self)->onReceive();
                    Serial.println("interrupt");
                },
                LOW,
                this);
        }

        /// @brief バスの有効性を取得
        operator bool() const
        {
            return true;
        }

        /// @brief 更新
        /// @param transmitIntervalMs 送信間隔 [ms]
        void update(uint32_t transmitIntervalMs = 5000)
        {
            if (txNodes.size() && micros() - timestampUs >= transmitIntervalMs)
            {
                onTransmit();
            }
            onReceive();    // TODO: 受信処理を割り込みに移行する
        }

        /// @brief バスの状態を表示する
        void show()
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

        void leaveTx(const CanNode& node) override
        {
            txNodes.erase(std::find(txNodes.begin(), txNodes.end(), &node));
        }

        void leaveRx(const CanNode& node) override
        {
            rxNodes.erase(std::find_if(rxNodes.begin(), rxNodes.end(), [&node](const RxNode& rxNode)
                                       { return rxNode.node == &node; }));
        }

    private:
        void onReceive()
        {
            can_frame msg;
            if (bus.readMessage(&msg) == MCP2515::ERROR_OK)
            {
                auto rxNode = std::find_if(rxNodes.begin(), rxNodes.end(), [msg](const RxNode& rx)
                                           { return rx.node->id == msg.can_id; });
                if (rxNode == rxNodes.end())
                {
                    return;
                }
                Udon::Detail::Unpacketize(
                    { const_cast<can_frame&>(msg).data },
                    { rxNode->node->data, rxNode->node->length },
                    SingleFrameSize);

                const size_t packetCount = static_cast<size_t>(std::ceil(
                    static_cast<double>(rxNode->node->length) / SingleFrameSize - 1 /*-1: index*/));

                if (rxNode->node->length > SingleFrameSize)
                {
                    if (msg.data[0] == packetCount)
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
        }
        void onTransmit()
        {
            for (auto&& node : txNodes)
            {
                can_frame msg{};
                msg.can_id  = node->id;
                msg.can_dlc = 8;
                Udon::Detail::Packetize(
                    { node->data, node->length },
                    { msg.data },
                    SingleFrameSize,
                    [this, &msg](size_t)
                    {
                        bus.sendMessage(&msg);
                    });
            }
        }
    };
}    // namespace Udon

#endif