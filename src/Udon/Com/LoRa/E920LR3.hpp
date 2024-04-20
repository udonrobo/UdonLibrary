//
//    E920LR3 LoRaモジュール ドライバクラス
//
//	  Copyright (c) 2022-2023 Fujimoto Ryo
//    Copyright (c) 2022-2023 Udonrobo
//
// FSK変調タイプもLoraとして書いておく
#pragma once

#include "ILoRa.hpp"

#include <Udon/Algorithm/BitPack.hpp>
#include <Udon/Types/Optional.hpp>
#include <Udon/Common/Printf.hpp>

namespace Udon
{
    class E920LR3
        : public ILoRa
    {
    public:
        struct E920Config
        {
            uint8_t  nodeType     = 1;         // ノードの種別 1:親機 2:子機 3:中継機
            uint8_t  ackMode      = 2;         // ACK 1:有効 2:無効
            uint8_t  retry        = 2;         // リトライ回数 0~10
            uint8_t  transMode    = 1;         // 転送方式 1:PayLoad 2:Frame
            uint8_t  rcvidrssi    = 2;         // データへの送信元情報、受信強度の付与 1:ON 2:OFF
            uint8_t  baudRate     = 5;         // UART速度 1:9600bps 2:19200bps 3:38400bps 4:57600bps 5:115200bps 6:230400bps
            uint8_t  sleepMode    = 1;         // 1:OFF 2:TimerWakeUp 3:INTWakeUp(TxContinue) 4:INTWakeUp(OnetimeTx) 5:UARTWakeUp
            uint8_t  power        = 13;        // 送信出力 -4~13dBm
            uint8_t  format       = 1;         // PayLoadのフォーマット 1:ASCII 2:BINARY
            // uint8_t  rfMode       = 1;         // 無線通信モード 1:TxRx 2:Tx Only
            // uint8_t  mculpMode    = 2;         // スリープ中のMCUの省電力モード 1:STOPMode 2:SLEEPMode
            // uint8_t  rflpMode     = 3;         // スリープ中の無線部のモード 1:Sleep with Cold Start 2:Sleep with Warm Start 3:Active
            // uint8_t  protocolMode = 3;         // 通信プロトコル 1:LoRa 2:LoRa with Static Routing 3:FSK
            uint8_t  bw           = 6;         // LoRaの帯域幅 3:62.5kHz 4:125kHz 5:250kHz
            uint8_t  sf           = 7;         // LoRaの拡散率 7~12
            uint8_t  channel      = 0x0A;      // LoRa:125khz以下&FSK:50kbps->1~38 other->1~19
            uint16_t panid        = 0x0001;    // 自ノードが参加するPANネットワークアドレス(ペアで共通) 0x0001~0xFFFE
            uint16_t ownid        = 0x0000;    // 自ノードのネットワークアドレス(自身が親機の場合0) 0x0000~0xFFFE
            uint16_t dstid        = 0x0002;    // 送信先ノードのネットワークアドレス(送信先が親機の場合0) 0x0000~0xFFFE
            // uint8_t  hopcount     = 1;         // 中継回数基本1 1~3
            // uint8_t  rate         = 3;         // FSKでのデータレート 1:50kbps 2:100kbps 3:150kbps
        };

        /// @brief コンストラクタ
        /// @param uart モジュールと接続しているUARTインスタンス
        E920LR3(HardwareSerial& uart);

        /// @brief 割り込みありのコンストラクタ
        /// @param uart
        /// @param INTPin WakeUp,Sleep用のためのピン
        E920LR3(HardwareSerial& uart, uint8_t INTPin);

        /// @brief 通信開始
        /// @param config 設定
        /// @param setMode 詳細まで設定するか true:する false:しない
        bool begin(const E920Config& e920config, const bool setMode = false);

        /// @brief 更新
        void update();

        void setParameter(String parameter, uint16_t value);


        /// @brief バスの状態を表示
        void show() const;

        /// @brief 通信中であるか
        /// @return 通信中であるならtrue
        operator bool() const override { return uart && not isTimeout(700); }

        /// @brief 送信ノードを登録
        void joinTx(LoRaNode& node) override { txNode = &node; }

        /// @brief 受信ノードを登録
        void joinRx(LoRaNode& node) override { rxNode = &node; }

    private:
        HardwareSerial& uart;

        Udon::Optional<uint8_t> INTPin;

        LoRaNode* txNode = nullptr;
        LoRaNode* rxNode = nullptr;

        E920Config e920config;

        enum class TransmitMode
        {
            Send,
            Receive,
            TwoWay,
            Empty,
        };

        /// @brief 通信モードを取得する
        TransmitMode getTransmitMode() const;


        bool sendUpdate();

        bool receiveUpdate();

        void twoWayUpdate();

        bool isTimeout(uint32_t timeoutMs) const;

        bool waitResponse();
    };

}    // namespace Udon

#include "Impl/E920LR3.hpp"
