/// @file   CanBusTeensy.hpp
/// @date   2023/01/13
/// @brief  FlexCan_T4ライブラリを用いたCanバス管理クラス
/// @author 大河 祐介

#pragma once

//                                   vvv teensy3.2 vvv        vvv teensy3.5 vvv         vvv teensy3.6 vvv       vvv teensy4.0/4.1 vvv
#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)

#include <FlexCAN_T4.h>       // https://github.com/tonton81/FlexCan_T4.git
#include <IntervalTimer.h>    // https://github.com/loglow/IntervalTimer.git

#include <udon/algorithm/RingBuffer.hpp>
#include <udon/com/can/CanInfo.hpp>
#include <udon/com/can/CanUtility.hpp>
#include <udon/com/can/ICanBus.hpp>

namespace udon
{

    /// @tparam {Bus} バス種類 (Can0,Can1,Can2,Can3)
    template <CAN_DEV_TABLE Bus>
    class CanBusTeensy
        : public ICanBus
    {

        FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;
        IntervalTimer                             isr;

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

        udon::RingBuffer<Can20ANode, 32> txNodes;
        udon::RingBuffer<Can20ANode, 64> rxNodes;

        udon::CanBusErrorInfo errorInfo;

        static CanBusTeensy* self;

    public:
        CanBusTeensy()
            : bus{}
            , isr{}
            , txNodes{}
            , rxNodes{}
        {
            self = this;
        }

        ~CanBusTeensy()
        {
            end();
        }

        /// @brief 通信開始
        /// @param {baudrate} 通信レート
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
                isr.begin(
                    []
                    {
                        for (int i = 0; i < 10; ++i)
                        {
                            self->bus.events();
                        }
                    },
                    100);
            }
        }

        /// @brief 通信終了
        void end()
        {
            isr.end();
            bus.disableFIFO();
            bus.disableFIFOInterrupt();
        }

        /// @brief バス更新
        /// @param {transmissionIntervalMs} 送信間隔
        void update(uint32_t transmissionIntervalMs = 5000)
        {
            if (txNodes.size() && micros() - errorInfo.timestampUs >= transmissionIntervalMs)
            {
                onTransmit();
            }
        }

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
            auto it = std::find_if(txNodes.begin(), txNodes.end(), [id](const Can20ANode& node)
                                   { return node.id == id; });
            if (it == txNodes.end())
            {
                // not found node => create new node
                txNodes.push({ id,
                               std::vector<uint8_t>(size) });
                return txNodes.back().view();
            }
            else
            {
                // return existing node
                return it->view();
            }
        }

        /// @brief RXノードを作成する
        /// @param id ノードID [11bit]
        /// @param size データサイズ [byte]
        /// @return CanNodeView RXノードバッファの参照
        CanNodeView createRxNode(uint16_t id, size_t size) override
        {
            auto it = std::find_if(rxNodes.begin(), rxNodes.end(), [id](const Can20ANode& node)
                                   { return node.id == id; });
            if (it == rxNodes.end())
            {
                // not found node => create new node
                rxNodes.push({ id,
                               std::vector<uint8_t>(size) });
                return rxNodes.back().view();
            }
            else
            {
                // return existing node
                return it->view();
            }
        }

    private:
        static void onReceive(const CAN_message_t& msg)
        {
            auto node = std::find_if(self->rxNodes.begin(), self->rxNodes.end(), [msg](const Can20ANode& node)
                                     { return node.id == msg.id; });
            if (node == self->rxNodes.end())
            {
                return;
            }
            udon::Unpacketize(
                { const_cast<CAN_message_t&>(msg).buf },
                { node->data.data(), node->data.size() },
                Can20ANode::SinglePacketSize);
        }

        void onTransmit()
        {
            for (auto&& node : txNodes)
            {
                CAN_message_t msg{};
                msg.id = node.id;
                udon::Packetize(
                    { node.data.data(), node.data.size() },
                    { msg.buf },
                    Can20ANode::SinglePacketSize,
                    [&msg]()
                    {
                        while (not self->bus.write(msg))
                            ;    // 送信
                        delayMicroseconds(200);
                    });
            }
        }
    };

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;

}    // namespace udon

#endif
