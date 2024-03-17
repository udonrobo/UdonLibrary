//
//    CAN通信 Raspberry Pi Pico用バスクラス 実装部
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

namespace Udon
{
    /// @brief コンストラクタ
    /// @param config 設定
    inline CanBusSpi::CanBusSpi(const Config& config)
        : config(config)
        , bus(/* spi_inst_t* CHANNEL    */ config.channel,
              /* uint8_t     CS_PIN     */ config.cs,
              /* uint32_t    _SPI_CLOCK */ config.spiClock)
    {
    }

    /// @brief 通信開始
    /// @note SPI通信も開始します。
    inline void CanBusSpi::begin()
    {
        (void)spi_init(/* spi_inst_t* spi      */ config.channel,
                       /* uint        baudrate */ config.spiClock);

        spi_set_format(/* spi_inst_t* spi       */ config.channel,
                       /* uint        data_bits */ 8,
                       /* spi_cpol_t  cpol      */ SPI_CPOL_0,
                       /* spi_cpha_t  cpha      */ SPI_CPHA_0,
                       /* spi_order_t order     */ SPI_MSB_FIRST);

        spi_set_slave(/* spi_inst_t* spi   */ config.channel,
                      /* bool        slave */ false);

        gpio_set_function(config.mosi, GPIO_FUNC_SPI);
        gpio_set_function(config.miso, GPIO_FUNC_SPI);
        gpio_set_function(config.sck, GPIO_FUNC_SPI);

        gpio_init(config.cs);
        gpio_set_dir(config.cs, true);
        gpio_put(config.cs, true);


        beginCanOnly();
    }

    /// @brief CAN通信のみ開始する
    /// @note SPI通信は別途開始する必要がある
    ///         SPIバスがCANコントローラーとの通信のみに使用される場合は、この関数を呼び出す必要はない
    inline void CanBusSpi::beginCanOnly()
    {
        bus.reset();

        // 受信開始
        if (const auto rxSize = rxNodes.size())
        {
            // 割り込み設定
            pinMode(config.interrupt, INPUT_PULLDOWN);
            attachInterruptParam(
                digitalPinToInterrupt(config.interrupt),
                [](void* p)
                {
                    auto      self = static_cast<CanBusSpi*>(p);
                    can_frame msg;
                    if (self->bus.readMessage(&msg) == MCP2515::ERROR_OK)
                    {
                        self->rxBuffer.push_back(msg);    // 受信データ追加
                    }
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

        bus.setBitrate(config.canBaudrate, config.mcpClock);

        bus.setNormalMode();
    }

    /// @brief 通信終了
    inline void CanBusSpi::end()
    {
        bus.reset();
    }

    /// @brief バスの有効性を取得
    inline CanBusSpi::operator bool() const
    {
        return not(txTimeout() or rxTimeout());
    }

    inline bool CanBusSpi::txTimeout() const
    {
        if (txNodes)
            return millis() - transmitMs >= config.transmitTimeout;
        else
            return false;
    }

    inline bool CanBusSpi::rxTimeout() const
    {
        if (rxNodes)
            return millis() - receiveMs >= config.receiveTimeout;
        else
            return false;
    }

    /// @brief バス更新
    inline void CanBusSpi::update()
    {
        onReceive();
        if (txNodes and millis() - transmitMs >= config.transmitInterval)
        {
            onTransmit();
            transmitMs = millis();
        }
    }

    /// @brief バスの状態を表示する
    inline void CanBusSpi::show() const
    {
        Serial.print("CanBusSpiPico\n");

        for (auto&& node : txNodes)
        {
            Serial.print("\tTX  ");

            Serial.printf("0x%03x ", static_cast<int>(node->id));

            Serial.printf("%2zu byte ", node->length);

            Serial.print(node->length > SingleFrameSize ? "(multi frame) " : "(single frame) ");

            Serial.print("[");
            for (size_t i = 0; i < node->length; ++i)
            {
                Serial.printf("%4d", node->data[i]);
            }
            Serial.print(" ]");

            Serial.println();
        }

        for (auto&& rxNode : rxNodes)
        {
            Serial.print("\tRX  ");

            Serial.printf("0x%03x ", static_cast<int>(rxNode.node->id));

            Serial.printf("%2zu byte ", rxNode.node->length);

            Serial.print(rxNode.node->length > SingleFrameSize ? "(multi frame) " : "(single frame) ");

            Serial.print("[");
            for (size_t i = 0; i < rxNode.node->length; ++i)
            {
                Serial.printf("%4d", rxNode.node->data[i]);
            }
            Serial.print(" ]");

            Serial.println();
        }
    }

    /// @brief 送信ノードをバスに参加させる
    /// @param node 送信ノード
    inline void CanBusSpi::joinTx(CanNode& node)
    {
        txNodes.push_back(&node);
    }

    /// @brief 受信ノードをバスに参加させる
    /// @param node 受信ノード
    inline void CanBusSpi::joinRx(CanNode& node, void (*onReceive)(void*), void* p)
    {
        rxNodes.push_back({ &node, onReceive, p });
    }

    /// @brief 送信ノードをバスから離脱させる
    /// @note 送信ノードのインスタンスポインタを基に削除します。
    /// @param node 送信ノード
    inline void CanBusSpi::leaveTx(const CanNode& node)
    {
        txNodes.erase(std::find(txNodes.begin(), txNodes.end(), &node));
    }

    /// @brief 受信ノードをバスから離脱させる
    /// @note 受信ノードのインスタンスポインタを基に削除します。
    /// @param node 受信ノード
    inline void CanBusSpi::leaveRx(const CanNode& node)
    {
        rxNodes.erase(std::find_if(rxNodes.begin(), rxNodes.end(), [&node](const RxNodePtr& rxNode)
                                   { return rxNode.node == &node; }));
    }

    inline void CanBusSpi::onReceive()
    {
        for (auto&& msg : rxBuffer)
        {
            // IDに対応する受信ノードを探す
            auto rxNode = std::find_if(rxNodes.begin(), rxNodes.end(), [msg](const RxNodePtr& rx)
                                       { return rx.node->id == msg.can_id; });
            if (rxNode == rxNodes.end())
            {
                return;
            }

            // 分割されたフレームを結合(マルチフレームの場合)
            Udon::Impl::Unpacketize({ msg.data }, { rxNode->node->data, rxNode->node->length }, SingleFrameSize);

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
        rxBuffer.clear();
    }
    inline void CanBusSpi::onTransmit()
    {
        for (auto&& node : txNodes)
        {
            can_frame msg{};
            msg.can_id  = node->id;
            msg.can_dlc = SingleFrameSize;

            // 一度に8バイトしか送れないため、分割し送信
            Udon::Impl::Packetize({ node->data, node->length }, { msg.data }, SingleFrameSize,
                                    [this, &msg](size_t)
                                    {
                                        bus.sendMessage(&msg);
                                        delayMicroseconds(200);
                                    });

            node->transmitMs = millis();
        }
    }
}    // namespace Udon
