//
//    TLM922S LoRaモジュール デバイスドライバー
//
//    Copyright (c) 2022-2023 Udonrobo
//
//    データシート: https://usermanual.wiki/Kiwi-Technology/TLM922S/pdf
//

#pragma once

#include "ILoRa.hpp"
#include <Udon/Com/Common/AsyncStreamReader.hpp>

namespace Udon
{

    /// @brief TLM922S デバイスクラス (プライベートLoRa)
    /// @note 現在送受信非対応
    class TLM922S
        : public ILoRa
    {
    public:
        /// @brief 設定項目
        struct TlmConfig
        {
            uint32_t frequency;                // 周波数[Hz] (902000000 ~ 928000000)
            uint8_t  power           = 13;     // 送信電力[dBm] (2~20)
            uint8_t  spreadingFactor = 7;      // 拡散係数 (7, 8, 9, 10, 11, 12)
            uint16_t bandwidth       = 500;    // 帯域幅[kHz] (125, 250, 500)
        };

        /// @brief コンストラクタ
        /// @param uart モジュールと接続しているStreamインスタンス
        TLM922S(HardwareSerial& uart);

        operator bool() const override
        {
            return true;
        }

        /// @brief 通信開始
        /// @param config
        void begin(const TlmConfig& config);

        /// @brief 更新
        void update();

        /// @brief 送信ノードを登録
        void joinTx(LoRaNode& node) override { txNode = &node; }

        /// @brief 受信ノードを登録
        void joinRx(LoRaNode& node) override { rxNode = &node; }

    private:
        std::vector<uint8_t> HexStringToBinString(ArrayView<const char> view);

        HardwareSerial& uart;

        AsyncStreamReader reader;

        LoRaNode* txNode = nullptr;
        LoRaNode* rxNode = nullptr;
    };

}    // namespace Udon

#include "Impl/TLM922S.hpp"
