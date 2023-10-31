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

#    include <Udon/Types/Pin.hpp>

#    include <Udon/Algorithm/StaticVector.hpp>
#    include <Udon/Thirdparty/pico_mcp2515/mcp2515.h>

namespace Udon
{
    /// @brief Raspberry Pi Pico用バスクラス
    class CanBusSpi
        : public ICanBus
    {
    public:
        struct SpiConfig
        {
            spi_inst_t* channel;    // SPI チャンネル (spi0, spi1)

            Pin cs;           // チップセレクトピン
            Pin interrupt;    // 受信割り込みピン
            Pin mosi;         // MOSIピン (TX)
            Pin miso;         // MISOピン (RX)
            Pin sck;          // クロックピン

            uint32_t clock = 1'000'000;    // SPIクロック周波数 [Hz]
        };

        struct CanConfig
        {
            CAN_SPEED speed    = CAN_1000KBPS;    // CAN通信速度
            CAN_CLOCK mcpClock = MCP_16MHZ;       // トランシーバー動作クロック周波数 [Hz]
        };

        /// @brief コンストラクタ
        /// @param spiConfig SPI 設定情報
        /// @param canConfig CAN 設定情報
        CanBusSpi(const SpiConfig& spiConfig, const CanConfig& canConfig);

        /// @brief 通信開始
        /// @remark SPI通信も開始します。
        void begin();

        /// @brief CAN通信のみ開始する
        /// @remark SPI通信は別途開始する必要がある
        ///         SPIバスがCANコントローラーとの通信のみに使用される場合は、この関数を呼び出す必要はない
        /// @param interrupt 割り込みピン
        /// @param canConfig CAN 設定情報
        void beginCanOnly(Pin interrupt, const CanConfig& canConfig);

        /// @brief 通信終了
        void end();

        /// @brief バスの有効性を取得
        explicit operator bool() const override;

        /// @brief バス更新
        /// @param transmitIntervalUs 送信間隔 [us]
        void update(uint32_t transmitIntervalUs = 5000);

        /// @brief バスの状態を表示する
        void show() const;
        

        void joinTx(CanNode& node) override;

        void joinRx(CanNode& node, void (*onReceive)(void*), void* p) override;

        void leaveTx(const CanNode& node) override;

        void leaveRx(const CanNode& node) override;

    private:
        SpiConfig spiConfig;
        CanConfig canConfig;

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
                    onReceive(p);
            }
        };

        Udon::StaticVector<TxNodePtr> txNodes;
        Udon::StaticVector<RxNodePtr> rxNodes;

        Udon::StaticVector<can_frame, 1024> rxBuffer;

        uint32_t transmitUs = 0;
        uint32_t receiveMs  = 0;

        void onReceive();
        void onTransmit();
    };
}    // namespace Udon

#    include "Impl/CanBusSpiPico.hpp"

#endif