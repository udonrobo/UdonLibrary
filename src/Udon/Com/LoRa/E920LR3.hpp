//
//    E920LR3 LoRaモジュール ドライバクラス
//
//	  Copyright (c) 2022-2023 Fujimoto Ryo
//    Copyright (c) 2022-2023 Udonrobo
//
//FSK変調タイプもLoraとして書いておく
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
        /// @brief コンストラクタ
        /// @param uart モジュールと接続しているUARTインスタンス
        E920LR3(HardwareSerial& uart);

        /// @brief 割り込みありのコンストラクタ
        /// @param uart
        /// @param INTPin WakeUp,Sleep用のためのピン
        E920LR3(HardwareSerial& uart, uint8_t INTPin);

        /// @brief 通信開始
        bool begin(Config& config);

        /// @brief 更新
        void update();

        /// @brief バスの状態を表示
        void show() const;

        /// @brief 通信中であるか
        /// @return 通信中であるならtrue
        operator bool() const override { return uart && not isTimeout(700); }

        /// @brief 使用可能なチャンネルの範囲に値を収める
        /// @param channel 収める前のチャンネル数
        /// @return 収めた後のチャンネル数
        static int ClampChannel(int channel) { return constrain(channel, 0, 30); }

        /// @brief 送信ノードを登録
        void joinTx(LoRaNode& node) override { txNode = &node; }

        /// @brief 受信ノードを登録
        void joinRx(LoRaNode& node) override { rxNode = &node; }

    private:
        HardwareSerial& uart;

        Udon::Optional<uint8_t> INTPin;

        LoRaNode* txNode = nullptr;
        LoRaNode* rxNode = nullptr;

        enum class TransmitMode
        {
            Send,
            Receive,
            TwoWay,
            Empty,
        };

        struct Config
        {
            uint8_t  transmitMode;
            uint8_t  channel;
            uint16_t myNodeAddress;
            uint8_t myNodeType;
            uint8_t ackMode;
            uint8_t retry;
            uint8_t baudRate;
            uint8_t  sleepMode;
            uint8_t  power;
            uint8_t  format;
            uint8_t  rfMode;
            uint8_t  protocolMode;

        }config;

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
