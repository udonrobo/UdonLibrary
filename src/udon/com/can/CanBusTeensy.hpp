/// @file   CanBusTeensy.hpp
/// @date   2023/01/13
/// @brief  FlexCan_T4ライブラリを用いたCanバス管理クラス
/// @author 大河 祐介

#pragma once

//        vvv teensy3.2 vvv        vvv teensy3.5 vvv         vvv teensy3.6 vvv        vvv teensy4.0/4.1 vvv
#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)

#    include <FlexCAN_T4.h>       // https://github.com/tonton81/FlexCan_T4.git
#    include <IntervalTimer.h>    // https://github.com/loglow/IntervalTimer.git

#    include <udon/algorithm/RingBuffer.hpp>
#    include <udon/com/can/CanInfo.hpp>
#    include <udon/com/can/CanUtility.hpp>
#    include <udon/com/can/ICanBus.hpp>

namespace udon
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

#    if defined(__MK20DX256__) || defined(__MK64FX512__)
        static_assert(Bus == CAN0, "Only CAN0 works on Teensy 3.2/3.5");
#    endif
#    if defined(__MK66FX1M0__)
        static_assert(Bus == CAN0 || Bus == CAN1, "Only CAN0 & CAN1 works on Teensy 3.6");
#    endif
#    if defined(__IMXRT1062__)
        static_assert(Bus == CAN1 || Bus == CAN2 || Bus == CAN3, "Only CAN1 & CAN2 & CAN3 works on Teensy 4.0/4.1");
#    endif

        FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;

        IntervalTimer isr;

        struct Can20ANode
        {
            static constexpr size_t SinglePacketSize = 8;

            uint16_t id;    // id: 11bit

            std::vector<uint8_t> data;    // データ

            std::vector<uint8_t> temp;    // 一時的にデータを格納するバッファ(マルチフレームの場合に使用)

            uint32_t transmitUs;

            CanNodeView view()
            {
                return { id, &data, &transmitUs };
            }
        };

        udon::RingBuffer<Can20ANode, 32> txNodes = {};
        udon::RingBuffer<Can20ANode, 64> rxNodes = {};

        uint32_t transmitUs = 0;

        static CanBusTeensy* self;

    public:
        /// @brief コンストラクタ
        CanBusTeensy()
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
        void begin(const uint32_t baudrate = 1'000'000)
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
                        for (int i = 0; i < 1; ++i)
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
            return micros() - transmitUs < 100'000;
        }

        void show() const
        {
            PrintBusInfo("CAN 2.0A", txNodes, rxNodes, Can20ANode::SinglePacketSize);
        }

        /// @brief TXノードを作成する
        /// @param id ノードID [11bit]
        /// @param size データサイズ [byte]
        /// @return CanNodeView TXノードバッファの参照
        CanNodeView createTxNode(uint32_t id, size_t size) override
        {
            return createNode(static_cast<uint16_t>(id), size, txNodes);
        }

        /// @brief RXノードを作成する
        /// @param id ノードID [11bit]
        /// @param size データサイズ [byte]
        /// @return CanNodeView RXノードバッファの参照
        CanNodeView createRxNode(uint32_t id, size_t size) override
        {
            return createNode(static_cast<uint16_t>(id), size, rxNodes);
        }

    private:
        /// @brief 仮想ノード作成
        /// @tparam N
        /// @param id
        /// @param size
        /// @param nodes
        /// @return
        template <size_t N>
        CanNodeView createNode(uint16_t id, size_t size, udon::RingBuffer<Can20ANode, N>& nodes)
        {
            auto it = std::find_if(nodes.begin(), nodes.end(), [id](const Can20ANode& node)
                                   { return node.id == id; });
            if (it == nodes.end())
            {
                // 仮想ノードがない場合は新規作成する
                if (size > Can20ANode::SinglePacketSize)
                {
                    // マルチフレームの場合は一時的にデータを格納するバッファを確保する
                    nodes.push({
                        id,                            // id
                        std::vector<uint8_t>(size),    // data
                        std::vector<uint8_t>(size),    // temp
                    });
                }
                else
                {
                    // シングルフレームの場合は一時的にデータを格納するバッファは不要
                    nodes.push({
                        id,                            // id
                        std::vector<uint8_t>(size),    // data
                        std::vector<uint8_t>(),        // temp
                    });
                }
                return nodes.back().view();
            }
            else
            {
                // サイズが異なる場合はリサイズする(良くない)
                if (it->data.size() != size)
                {
                    it->data.resize(max(it->data.size(), size));
                }

                // 既存の仮想ノードがある場合はそれを返す
                return it->view();
            }
        }

        /// @brief 受信コールバック
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
                { node->temp.data(), node->temp.size() },
                Can20ANode::SinglePacketSize);

            const size_t packetCount = static_cast<size_t>(std::ceil(
                static_cast<double>(node->data.size()) / Can20ANode::SinglePacketSize - 1 /*-1: index*/));

            if (node->data.size() > Can20ANode::SinglePacketSize && msg.buf[0] == packetCount)
            {
                // マルチフレームの最終フレームの場合はデータをコピーする
                // この動作を行わないと欠落したデータを取得することになる
                std::copy(node->temp.begin(), node->temp.end(), node->data.begin());
            }
        }

        /// @brief 送信処理
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
                    [&msg](size_t size)
                    {
                        msg.len = static_cast<uint8_t>(8);
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
