//
//    CAN通信 Teensy用バスクラス 実装部
//
//    Copyright (c) 2022-2023 udonrobo
//
#include <vector>
namespace Udon
{

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>::CanBusTeensy(const Config& config)
        : config(config)
    {
        self = this;
    }

    /// @brief コピーコンストラクタ
    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>::CanBusTeensy(const CanBusTeensy& other)
        : config(other.config)
        , bus(other.bus)
        , txNodes(other.txNodes)
        , rxNodes(other.rxNodes)
    {
        self = this;
    }

    /// @brief デストラクタ
    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>::~CanBusTeensy()
    {
        end();
    }

    /// @brief 通信開始
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::begin()
    {
        // バス初期化
        bus.begin();
        bus.setBaudRate(config.canBaudrate);

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
                        /* ID           */ rxNodes[i].id,
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
                    self->rxBuffer.push_back(msg);
                });
        }
    }

    /// @brief 通信終了
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::end()
    {
        bus.disableFIFOInterrupt();
        bus.disableFIFO();
    }

    /// @brief バス更新
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::update()
    {
        onReceive();
        if (txNodes.size() && millis() - transmitMs >= config.transmitInterval)
        {
            onTransmit();
            transmitMs = millis();
        }
    }

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>::operator bool() const
    {
        return not(txTimeout() or rxTimeout());
    }

    template <CAN_DEV_TABLE Bus>
    bool CanBusTeensy<Bus>::txTimeout() const
    {
        if (txNodes.size())
            return millis() - transmitMs >= config.transmitTimeout;
        else
            return false;
    }

    template <CAN_DEV_TABLE Bus>
    bool CanBusTeensy<Bus>::rxTimeout() const
    {
        if (rxNodes.size())
            return millis() - receiveMs >= config.receiveTimeout;
        else
            return false;
    }

    /// @brief バス情報を表示する
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::show() const
    {
        Serial.print("CanBusTeensy\n");

        for (auto&& node : txNodes)
        {
            Serial.print("\tTX  ");

            Serial.printf("0x%03x ", static_cast<int>(node.id));

            Serial.printf("%2zu byte ", node.data.size());

            Serial.print(node.data.size() > SingleFrameSize ? "(multi  frame) " : "(single frame) ");

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

            Serial.print(rxNode.data.size() > SingleFrameSize ? "(multi  frame) " : "(single frame) ");

            Serial.print("[");

            for (const auto& data : rxNode.data)
            {
                Serial.printf("%4d", data);
            }

            Serial.print(" ]");

            Serial.println();
        }
    }

    /// @brief 送信ノードをバスに参加させる
    /// @param node 送信ノード
    template <CAN_DEV_TABLE Bus>
    CanTxNode* CanBusTeensy<Bus>::createTx(uint32_t id, size_t length)
    {
        auto it = std::find_if(txNodes.begin(), txNodes.end(), [id](const CanTxNode& tx)
                               { return tx.id == id; });
        if (it == txNodes.end())
        {
            txNodes.push_back({ id, std::vector<uint8_t>(length), 0 });
            return &txNodes.back();
        }
        return &(*it);
    }

    /// @brief 受信ノードをバスに参加させる
    /// @param node 受信ノード
    template <CAN_DEV_TABLE Bus>
    CanRxNode* CanBusTeensy<Bus>::createRx(uint32_t id, size_t length)
    {
        rxNodes.push_back({
            id,
            std::vector<uint8_t>(length),
            nullptr,
            nullptr,
            0,
        });
        return &rxNodes.back();
    }

    /// @brief 受信処理
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::onReceive()
    {
        const auto ms = millis();

        for (auto&& msg : rxBuffer)
        {
            // IDに対応する受信ノードを探す
            auto rxNode = std::find_if(rxNodes.begin(), rxNodes.end(), [&msg](const CanRxNode& rx)
                                       { return rx.id == msg.id; });
            if (rxNode == rxNodes.end())
            {
                continue;
            }

            // 分割されたフレームを結合(マルチフレームの場合)
            Udon::Impl::Unpacketize(
                { msg.buf },
                { rxNode->data },
                SingleFrameSize);

            // 登録されている受信クラスのコールバック関数を呼ぶ
            // 最終フレームの到達時にコールバックを呼ぶため、受信中(完全に受信しきっていないとき)にデシリアライズすることを防いでいる。

            if (rxNode->data.size() > SingleFrameSize)
            {
                // マルチフレーム
                const auto frameCount = std::ceil(static_cast<double>(rxNode->data.size()) /( SingleFrameSize - 1 /*index*/) )- 1;

                if (msg.buf[0] == frameCount)
                {
                    rxNode->callback();
                }
            }
            else
            {
                // シングルフレーム
                rxNode->callback();
            }

            receiveMs = rxNode->transmitMs = ms;
        }
        rxBuffer.clear();
    }

    /// @brief 送信処理
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::onTransmit()
    {
        const auto ms = millis();

        for (auto&& node : txNodes)
        {
            CAN_message_t msg;
            msg.id = node.id;
            msg.len = SingleFrameSize;

            // 一度に8バイトしか送れないため、分割し送信
            Udon::Impl::Packetize({ node.data }, { msg.buf }, SingleFrameSize,
                                  [this, &msg](size_t)
                                  {
                                      bus.write(msg);
                                      delayMicroseconds(200);
                                  });

            node.transmitMs = ms;
        }
    }

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;

};    // namespace Udon
