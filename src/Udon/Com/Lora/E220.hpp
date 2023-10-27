//
//    UdonLibrary
//
//	  Copyright (c) 2022-2023 Fujimoto Ryo
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 Udonrobo
//
//    E220 Loraモジュール ドライバクラス
//

#pragma once

#include "ILora.hpp"

#include <Udon/Algorithm/BitPack.hpp>
#include <Udon/Stl/Optional.hpp>
#include <Udon/Common/Printf.hpp>

namespace Udon
{
    //
    //   E220 Loraモジュール
    //
    class E220
        : public ILora
    {
    public:
        /// @brief コンストラクタ
        /// @param uart モジュールと接続しているUARTインスタンス
        /// @param M0Pin 
        /// @param M1Pin 
        E220(HardwareSerial& uart, uint8_t M0Pin, uint8_t M1Pin);

        E220(HardwareSerial& uart, uint8_t M0Pin, uint8_t M1Pin, uint8_t AUXPin);

        /// @brief 通信開始
        /// @param channel チャンネル番号
        /// @param myAddress 自身のアドレス
        /// @param targetAddress 送信先のアドレス
        bool begin(uint8_t channel, uint16_t myAddress, uint16_t targetAddress);

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
        void joinTx(LoraNode& node) override { txNode = &node; }

        /// @brief 受信ノードを登録
        void joinRx(LoraNode& node) override { rxNode = &node; }

    private:
        HardwareSerial& uart;

        uint8_t  channel       = 0;
        uint16_t targetAddress = 0;

        uint8_t                 M0Pin;
        uint8_t                 M1Pin;
        Udon::Optional<uint8_t> AUXPin;

        LoraNode* txNode = nullptr;
        LoraNode* rxNode = nullptr;

        uint32_t lastWaitUntilCommandAcceptMs = 0;

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

        void waitUntilCommandAccept();
    };

}    // namespace Udon

#include "Detail/E220.hpp"
