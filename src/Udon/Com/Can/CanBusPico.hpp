#pragma once

#include <Arduino.h>

#if defined(ARDUINO_ARCH_RP2040)

#    include <stdint.h>
#    include <Udon/Algorithm/Delegate.hpp>
#    include <Udon/Algorithm/RingBuffer.hpp>
#    include <Udon/Com/Can/CanInfo.hpp>
#    include <Udon/Com/Can/CanUtility.hpp>
#    include <Udon/Com/Can/ICanBus.hpp>
#    include <udon/pio/PicoPioClock.hpp>

#    include <udon/thirdparty/PicoMcp2515/mcp2515.h>

namespace Udon
{
    class CanBusPico
        : public ICanBus
    {
        using Delegate = Udon::Delegate<CanBusPico, void()>;

        Delegate delegate;

        MCP2515 bus;

        struct Can20ANode
        {
            static constexpr size_t SinglePacketSize = 8;

            uint16_t id;    // id: 11bit

            std::vector<uint8_t> data;

            uint32_t timestamp;

            CanNodeView view()
            {
                return { id, &data, &timestamp };
            }
        };

        Udon::RingBuffer<Can20ANode, 32> txNodes;
        Udon::RingBuffer<Can20ANode, 64> rxNodes;

        Udon::CanBusErrorInfo errorInfo;

        uint8_t intPin;

    public:
        CanBusPico(
            uint8_t  intPin,
            uint8_t  mosiPin,
            uint8_t  misoPin,
            uint8_t  sckPin,
            uint8_t  csPin,
            uint32_t spiClock = 1000000)
            : delegate(this, &CanBusPico::onReceive)
            , bus(csPin, mosiPin, misoPin, sckPin, spiClock)
            , intPin(intPin)
        {
        }

        void begin(const CAN_SPEED canSpeed = CAN_1000KBPS, const CAN_CLOCK transceiverClock = MCP_8MHZ)
        {
            SPI.begin();
            bus.reset();
            bus.setBitrate(canSpeed, transceiverClock);
            bus.setNormalMode();
            pinMode(intPin, INPUT_PULLUP);
            attachInterrupt(
                digitalPinToInterrupt(intPin),
                delegate,
                FALLING);
        }

        /// @brief 更新
        /// @param transmitIntervalMs 送信間隔 [ms]
        void update(uint32_t transmitIntervalMs = 5000)
        {
            if (txNodes.size() && micros() - errorInfo.timestampUs >= transmitIntervalMs)
            {
                onTransmit();
            }
            onReceive();    // TODO: 受信処理を割り込みに移行する
        }

        /// @brief バスの状態を表示する
        void show()
        {
            PrintBusInfo("CAN 2.0A", txNodes, rxNodes, Can20ANode::SinglePacketSize);
        }

        /// @brief TXノードを作成する
        /// @param id ノードID [11bit]
        /// @param size データサイズ [byte]
        /// @return CanNodeView TXノードバッファの参照
        CanNodeView createTxNode(uint16_t id, size_t size) override
        {
            auto node = std::find_if(txNodes.begin(), txNodes.end(), [id](const Can20ANode& node)
                                     { return node.id == id; });
            if (node == txNodes.end())
            {
                // not found node => create new node
                txNodes.push({ id,
                               std::vector<uint8_t>(size) });
                return txNodes.back().view();
            }
            else
            {
                // return existing node
                return node->view();
            }
        }

        /// @brief RXノードを作成する
        /// @param id ノードID [11bit]
        /// @param size データサイズ [byte]
        /// @return CanNodeView RXノードバッファの参照
        CanNodeView createRxNode(uint16_t id, size_t size) override
        {
            auto node = std::find_if(rxNodes.begin(), rxNodes.end(), [id](const Can20ANode& node)
                                     { return node.id == id; });
            if (node == rxNodes.end())
            {
                // not found node => create new node
                rxNodes.push({ id,
                               std::vector<uint8_t>(size) });
                return rxNodes.back().view();
            }
            else
            {
                // return existing node
                return node->view();
            }
        }

    private:
        void onReceive()
        {
            can_frame msg;
            if (bus.readMessage(&msg) == MCP2515::ERROR_OK)
            {
                auto node = std::find_if(rxNodes.begin(), rxNodes.end(), [msg](const Can20ANode& node)
                                         { return node.id == msg.can_id; });
                if (node == rxNodes.end())
                {
                    return;
                }
                Udon::Unpacketize(
                    { const_cast<can_frame&>(msg).data },
                    { node->data.data(), node->data.size() },
                    Can20ANode::SinglePacketSize);
            }
        }
        void onTransmit()
        {
            for (auto&& node : txNodes)
            {
                can_frame msg{};
                msg.can_id  = node.id;
                msg.can_dlc = 8;
                Udon::Packetize(
                    { node.data.data(), node.data.size() },
                    { msg.data },
                    Can20ANode::SinglePacketSize,
                    [this, &msg]()
                    {
                        bus.sendMessage(&msg);
                        // delayMicroseconds(100);
                    });
            }
        }
    };
}    // namespace Udon

#endif