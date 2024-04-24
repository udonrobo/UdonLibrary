//
//    CAN通信 Raspberry Pi Pico用バスクラス (SPI経由)
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <SPI.h>

#include "ICanBus.hpp"
#include "CanNode.hpp"
#include "CanUtility.hpp"

#include <Udon/Types/StaticVector.hpp>
#include <Udon/Thirdparty/pico_mcp2515/mcp2515.h>
#include <Udon/Algorithm/ScopedInterruptLocker.hpp>
#include <deque>
namespace Udon
{
    /// @brief Raspberry Pi Pico用バスクラス
    class CanBusSpi
        : public ICanBus
    {
    public:
        /// @brief 設定
        struct Config
        {
            spi_inst_t* channel = spi_default;         // SPI チャンネル (spi0, spi1)
            uint8_t cs = PICO_DEFAULT_SPI_CSN_PIN;     // チップセレクトピン
            uint8_t interrupt;                         // 受信割り込みピン
            uint8_t mosi = PICO_DEFAULT_SPI_TX_PIN;    // MOSIピン (TX)
            uint8_t miso = PICO_DEFAULT_SPI_RX_PIN;    // MISOピン (RX)
            uint8_t sck = PICO_DEFAULT_SPI_SCK_PIN;    // クロックピン

            uint32_t spiClock = 1'000'000;    // SPIクロック周波数 [Hz]

            uint32_t transmitInterval = 5;           // 送信間隔 [ms]
            uint32_t transmitTimeout = 100;          // 送信タイムアウト時間 [ms]
            uint32_t receiveTimeout = 100;           // 受信タイムアウト時間 [ms]
            CAN_SPEED canBaudrate = CAN_1000KBPS;    // CAN通信速度
            CAN_CLOCK mcpClock = MCP_16MHZ;          // トランシーバー動作クロック周波数 [Hz]
        };

        /// @brief コンストラクタ
        /// @param config 設定
        CanBusSpi(const Config& config);

        /// @brief 通信開始
        /// @remark 呼び出し必須
        /// @remark SPI通信も開始します。
        void begin();

        /// @brief バス更新
        /// @remark 呼び出し必須
        void update();

        /// @brief 通信終了
        void end();

        /// @brief バスの有効性を取得
        explicit operator bool() const;

        /// @brief バスの状態を表示する
        void show() const;

        CanTxNode* createTx(uint32_t id, size_t length) override;

        CanRxNode* createRx(uint32_t id, size_t length) override;


    private:
        Config config;

        MCP2515 bus;

        constexpr static uint32_t SingleFrameSize = 8;

        std::deque<CanTxNode> txNodes;
        std::deque<CanRxNode> rxNodes;

        Udon::StaticVector<can_frame, 1024> rxBuffer;

        uint32_t transmitMs = 0;
        uint32_t receiveMs = 0;

        void onReceive();
        void onTransmit();

        /// @brief 送信タイムアウト
        bool txTimeout() const;

        /// @brief 受信タイムアウト
        bool rxTimeout() const;
    };
}    // namespace Udon

#include "Impl/CanBusSpiPico.hpp"
