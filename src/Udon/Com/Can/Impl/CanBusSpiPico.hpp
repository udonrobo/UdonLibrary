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


        bus.reset();

        // 受信開始
        if (const auto rxSize = rxNodes.size())
        {
            // 割り込み設定
            pinMode(config.interrupt, INPUT);

            attachInterruptParam(
                digitalPinToInterrupt(config.interrupt),
                [](void* p)
                {
                    auto self = static_cast<CanBusSpi*>(p);
                    can_frame msg;
                    if (self->bus.readMessage(&msg) == MCP2515::ERROR_OK)
                    {
                        self->rxBuffer.push_back(msg);    // 受信データ追加
                    }
                },
                LOW,    // 送信処理中、割り込み禁止になるため FALLING による監視はしない (変化のタイミングを逃すため)
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
                        bus.setFilter(filters[i], false, rxNodes[i].node.id);
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
        if (txNodes.size())
            return millis() - transmitMs >= config.transmitTimeout;
        else
            return false;
    }

    inline bool CanBusSpi::rxTimeout() const
    {
        if (rxNodes.size())
            return millis() - receiveMs >= config.receiveTimeout;
        else
            return false;
    }

    /// @brief バス更新
    inline void CanBusSpi::update()
    {
        onReceive();
        if (txNodes.size() and millis() - transmitMs >= config.transmitInterval)
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

            Serial.printf("0x%03x ", static_cast<int>(node.id));

            Serial.printf("%2zu byte ", node.data.size());

            Serial.print(node.data.size() > SingleFrameSize ? "(multi frame) " : "(single frame) ");

            Serial.print("[");

            for (const auto& data : node.data)
            {
                Serial.printf("%4d", data);
            }

            Serial.print(" ]");

            Serial.println();
        }

        for (auto&& rxNode : rxNodes)
        {
            Serial.print("\tRX  ");

            Serial.printf("0x%03x ", static_cast<int>(rxNode.id));

            Serial.printf("%2zu byte ", rxNode.data.size());

            Serial.print(rxNode.data.size() > SingleFrameSize ? "(multi frame) " : "(single frame) ");

            Serial.print("[");

            for (const auto&& data : rxNode.data)
            {
                Serial.printf("%4d", data);
            }
            Serial.print(" ]");

            Serial.println();
        }
    }

    /// @brief 送信ノードをバスに参加させる / 送信バッファの作成
    /// @param node 送信ノード
    inline CanTxNode* CanBusSpi::createTx(uint32_t id, size_t length)
    {
        auto it = std::find_if(txNodes.begin(), txNodes.end(), [id](const CanTxNode& tx)
                               { return tx.id == id; });
        if (it == txNodes.end())
        {
            txNodes.push_back({ id, { length } });
            return &txNodes.back();
        }
        return &(*it);
    }

    /// @brief 受信ノードをバスに参加させる　/ 受信バッファの作成
    /// @param node 受信ノード
    inline CanRxNode* CanBusSpi::createRx(uint32_t id, size_t length)
    {
        rxNodes.push_back({
            {
                id,
                length,
            },
            nullptr,
            nullptr,
        });
        return &rxNodes.back();
    }

    inline void CanBusSpi::onReceive()
    {
        for (auto&& msg : rxBuffer)
        {
            // IDに対応する受信ノードを探す
            auto rxNode = std::find_if(rxNodes.begin(), rxNodes.end(), [msg](const CanRxNode& rx)
                                       { return rx.node.id == msg.can_id; });
            if (rxNode == rxNodes.end())
            {
                continue;
            }

            // 分割されたフレームを結合(マルチフレームの場合)
            Udon::Impl::Unpacketize({ msg.data }, { rxNode->node.data, rxNode->node.length }, SingleFrameSize);

            // 登録されている受信クラスのコールバック関数を呼ぶ
            // 最終フレームの到達時にコールバックを呼ぶため、受信中(完全に受信しきっていないとき)にデシリアライズすることを防いでいる。
            if (rxNode->node.length > SingleFrameSize)
            {
                // マルチフレーム
                const auto index = std::ceil(static_cast<double>(rxNode->node.length) / (SingleFrameSize - 1 /*index*/)) - 1;

                if (msg.data[0] == index)
                {
                    rxNode->callback();
                }
            }
            else
            {
                // シングルフレーム
                rxNode->callback();
            }

            receiveMs = rxNode->node.transmitMs = millis();
        }
        rxBuffer.clear();
    }
    inline void CanBusSpi::onTransmit()
    {
        for (auto&& node : txNodes)
        {
            can_frame msg{};
            msg.can_id = node.id;
            msg.can_dlc = SingleFrameSize;

            // 一度に8バイトしか送れないため、分割し送信
            Udon::Impl::Packetize({ node.data, node.length }, { msg.data }, SingleFrameSize,
                                  [this, &msg](size_t)
                                  {
                                      bus.sendMessage(&msg);
                                      delayMicroseconds(200);
                                  });

            node.transmitMs = millis();
        }
    }
}    // namespace Udon
