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
#    include <Udon/Thirdparty/pico_mcp2515/mcp2515.h>

namespace Udon
{
    /// @brief Raspberry Pi Pico用バスクラス
    class CanBusSpi
        : public ICanBus
    {
    public:
        /// @brief コンストラクタ
        /// @param spiChannel SPIチャンネル (spi0 or spi1)
        /// @param csPin      チップセレクトピン
        /// @param spiClock   SPIクロック周波数 (CANコントローラーとの通信速度)
        CanBusSpi(
            spi_inst_t* spiChannel,
            uint8_t     csPin,
            uint32_t    spiClock = 1000000);

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
            CAN_SPEED canSpeed         = CAN_1000KBPS);

        /// @brief CAN通信のみ開始する
        /// @remark SPI通信は別途開始する必要がある
        ///         SPIバスがCANコントローラーとの通信のみに使用される場合は、この関数を呼び出す必要はない
        /// @param intPin            割り込みピン
        /// @param transceiverClock  CANトランシーバーのクロック周波数
        /// @param canSpeed          CAN通信速度
        void beginCanOnly(uint8_t intPin, CAN_CLOCK transceiverClock = MCP_16MHZ, CAN_SPEED canSpeed = CAN_1000KBPS);

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