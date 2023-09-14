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
//    CAN通信 Raspberry Pi Pico用バスクラス (SPI経由)
//
//-------------------------------------------------------------------

#pragma once

#if defined(ARDUINO_ARCH_RP2040)

#    include <SPI.h>

#    include "ICanBus.hpp"
#    include "CanNode.hpp"
#    include "CanUtility.hpp"

#    include <Udon/Algorithm/StaticVector.hpp>
#    include <Udon/Algorithm/RingBuffer.hpp>

#    include <Udon/Thirdparty/pico_mcp2515/mcp2515.h>

namespace Udon
{
    class CanBusSpi
        : public ICanBus
    {
        MCP2515 bus;

        constexpr static uint32_t SingleFrameSize = 8;

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

        Udon::RingBuffer<can_frame, 256> txBuffer;

        uint32_t transmitUs = 0;
        uint32_t receiveMs  = 0;

    public:
        /// @brief コンストラクタ
        /// @param spiChannel SPIチャンネル (spi0 or spi1)
        /// @param csPin      チップセレクトピン
        /// @param spiClock   SPIクロック周波数 (CANコントローラーとの通信速度)
        CanBusSpi(
            spi_inst_t* spiChannel,
            uint8_t     csPin,
            uint32_t    spiClock = 1000000)
            : bus(
                  /* spi_inst_t* CHANNEL    */ spiChannel,
                  /* uint8_t     CS_PIN     */ csPin,
                  /* uint32_t    _SPI_CLOCK */ spiClock)
        {
        }

        /// @brief 通信開始
        /// @remark SPI通信も開始します。
        /// @param intPin            割り込みピン
        /// @param txPin             送信ピン (MOSI)
        /// @param rxPin             受信ピン (MISO)
        /// @param sckPin            クロックピン
        /// @param transceiverClock  CANトランシーバーのクロック周波数
        /// @param canSpeed          CAN通信速度
        void begin(
            uint8_t   intPin,
            uint8_t   txPin            = PICO_DEFAULT_SPI_TX_PIN,
            uint8_t   rxPin            = PICO_DEFAULT_SPI_RX_PIN,
            uint8_t   sckPin           = PICO_DEFAULT_SPI_SCK_PIN,
            CAN_CLOCK transceiverClock = MCP_16MHZ,
            CAN_SPEED canSpeed         = CAN_1000KBPS)
        {
            SPIClassRP2040 spi{
                /* spi_inst_t *spi */ bus.getChannel(),
                /* pin_size_t rx   */ rxPin,
                /* pin_size_t cs   */ bus.getCS(),
                /* pin_size_t sck  */ sckPin,
                /* pin_size_t tx   */ txPin
            };    // todo: 開始するために一時的に生成するのはちょっとキモイ
            spi.begin();
            beginCanOnly(intPin, transceiverClock, canSpeed);
        }

        /// @brief CAN通信のみ開始する
        /// @remark SPI通信は別途開始する必要がある
        ///         SPIバスがCANコントローラーとの通信のみに使用される場合は、この関数を呼び出す必要はない
        /// @param intPin            割り込みピン
        /// @param transceiverClock  CANトランシーバーのクロック周波数
        /// @param canSpeed          CAN通信速度
        void beginCanOnly(uint8_t intPin, CAN_CLOCK transceiverClock = MCP_16MHZ, CAN_SPEED canSpeed = CAN_1000KBPS)
        {
            bus.reset();

            // 受信開始
            if (const auto rxSize = rxNodes.size())
            {
                // 割り込み設定
                pinMode(intPin, INPUT_PULLDOWN);
                attachInterruptParam(
                    digitalPinToInterrupt(intPin),
                    [](void* self)
                    {
                        static_cast<CanBusSpi*>(self)->onReceive();
                    },
                    LOW,
                    this);

                // 受信フィルタ設定 (ノード数が6以下の場合のみ)
                constexpr size_t Mcp2515MaxFilterCount = 6;
                if (rxSize <= Mcp2515MaxFilterCount)
                {
                    bus.setFilterMask(MCP2515::MASK0, false, 0x7FF);
                    bus.setFilterMask(MCP2515::MASK1, false, 0x7FF);

                    constexpr MCP2515::RXF filters[] = { MCP2515::RXF0, MCP2515::RXF1, MCP2515::RXF2, MCP2515::RXF3, MCP2515::RXF4, MCP2515::RXF5 };

                    for (size_t i = 0; i < Mcp2515MaxFilterCount; ++i)
                    {
                        if (i < rxSize)
                            bus.setFilter(filters[i], false, rxNodes[i].node->id);
                        else
                            bus.setFilter(filters[i], false, 0x7FF);    // 未使用のフィルタは全て0x7FFに設定
                    }
                }
            }

            bus.setBitrate(canSpeed, transceiverClock);

            if (rxNodes && not txNodes)
                bus.setListenOnlyMode();    // 受信のみの場合は受信モードに設定 (送受信モードのマイコンが再起動したとき、全ノードが停止したため。)
            else
                bus.setNormalMode();
        }

        /// @brief 通信終了
        void end()
        {
            bus.reset();
        }

        /// @brief バスの有効性を取得
        explicit operator bool() const override
        {
            if (rxNodes)
                return millis() - receiveMs < 100;
            else if (txNodes)
                return micros() - transmitUs < 100000;
            else
                return false;
        }

        /// @brief バス更新
        /// @param transmitIntervalUs 送信間隔 [us]
        void update(uint32_t transmitIntervalUs = 5000)
        {
            if (txNodes && micros() - transmitUs >= transmitIntervalUs)
            {
                onTransmit();
                transmitUs = micros();
            }
        }

        /// @brief バスの状態を表示する
        void show()
        {
            Serial.print("Bus: CAN 2.0B\n");

            Serial.print("\tTX Node\n");
            for (auto&& node : txNodes)
            {
                Serial.printf("\t\tid: 0x%-3x length:%3zu byte", static_cast<int>(node->id), node->length);
                if (node->length > SingleFrameSize)
                {
                    Serial.print(" (multi frame)");
                }
                else
                {
                    Serial.print(" (single frame)");
                }

                Serial.print("  data: ");
                for (size_t i = 0; i < node->length; ++i)
                {
                    Serial.printf("%4d", node->data[i]);
                }

                Serial.println();
            }

            Serial.print("\tRX Node\n");
            for (auto&& rxNode : rxNodes)
            {
                Serial.printf("\t\tid: 0x%-3x length:%3zu byte", static_cast<int>(rxNode.node->id), rxNode.node->length);
                if (rxNode.node->length > SingleFrameSize)
                {
                    Serial.print(" (multi frame)");
                }
                else
                {
                    Serial.print(" (single frame)");
                }

                Serial.print("  data: ");
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
        /// @brief 受信割り込み
        void onReceive()
        {
            // 受信データ取得
            can_frame msg;
            if (bus.readMessage(&msg) != MCP2515::ERROR_OK)
            {
                return;
            }

            // IDに対応する受信ノードを探す
            auto rxNode = std::find_if(rxNodes.begin(), rxNodes.end(), [msg](const RxNodePtr& rx)
                                       { return rx.node->id == msg.can_id; });
            if (rxNode == rxNodes.end())
            {
                return;
            }

            // 分割されたフレームを結合(マルチフレームの場合)
            Udon::Detail::Unpacketize({ msg.data }, { rxNode->node->data, rxNode->node->length }, SingleFrameSize);

            // 登録されている受信クラスのコールバック関数を呼ぶ
            // 最終フレームの到達時にコールバックを呼ぶため、受信中(完全に受信しきっていないとき)にデシリアライズすることを防いでいる。
            if (rxNode->node->length > SingleFrameSize)
            {
                // マルチフレーム
                const auto frameCount = std::ceil(static_cast<double>(rxNode->node->length) / SingleFrameSize - 1 /*index*/);

                if (msg.data[0] == frameCount)
                {
                    rxNode->callback();
                }
            }
            else
            {
                // シングルフレーム
                rxNode->callback();
            }

            receiveMs = rxNode->node->transmitMs = millis();
        }
        void onTransmit()
        {
            for (auto&& node : txNodes)
            {
                can_frame msg{};
                msg.can_id  = node->id;
                msg.can_dlc = SingleFrameSize;

                // 一度に8バイトしか送れないため、分割し送信
                Udon::Detail::Packetize({ node->data, node->length }, { msg.data }, SingleFrameSize,
                                        [this, &msg](size_t)
                                        {
                                            bus.sendMessage(&msg);
                                            delayMicroseconds(200);
                                        });

                node->transmitMs = millis();
            }
        }
    };
}    // namespace Udon

#endif