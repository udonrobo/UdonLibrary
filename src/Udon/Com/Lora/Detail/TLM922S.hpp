//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 Udonrobo
//
//    TLM922S LoRaモジュール デバイスドライバー 実装部
//
//    データシート: https://usermanual.wiki/Kiwi-Technology/TLM922S/pdf
//

#pragma once

namespace Udon
{

    inline TLM922S::TLM922S(HardwareSerial& uart)
        : uart(uart)
        , reader(uart)
    {
    }

    inline void TLM922S::begin(const TlmConfig& config)
    {
        uart.begin(115200);

        // エコーバックオフ
        uart.print("mod set_echo off\r\n");
        delay(100);

        // 周波数設定
        uart.printf("p2p set_freq %d\r\n", config.frequency);
        delay(100);

        // 送信電力設定
        uart.printf("p2p set_pwr %d\r\n", config.power);
        delay(100);

        // 帯域幅設定
        uart.printf("p2p set_bw %d\r\n", config.bandwidth);
        delay(100);

        // 拡散係数設定
        uart.printf("p2p set_sf %d\r\n", config.spreadingFactor);
    }

    inline void TLM922S::update()
    {
        // 送信処理
        if (txNode)
        {
            if (millis() - txNode->transmitMs > 40)    // 40msが程よい送信間隔
            {
                uart.print("p2p tx ");
                for (int i = 0; i < txNode->size; ++i)
                {
                    uart.printf("%02x", txNode->data[i]);
                }
                uart.print("\r\n");
                txNode->transmitMs = millis();
                // todo: レスポンスを確認する
            }
        }

        // 受信処理
        if (rxNode)
        {
            // 受信文字列解析
            if (const auto receiveStringView = reader.readStringUntil('\n').subView(1 /*先頭文字[\n]*/))
            {
                if (receiveStringView.startsWith(">> radio_rx "))    // 受信フォーマット \n>> radio_rx 5432abac(data) -90(RSSI) -50(SNR)
                {
                    const auto dataView = receiveStringView
                                              .subView(12)           // strlen(">> radio_rx ")
                                              .subViewUntil(' ');    // データフィールドとRSSI値間の空白まで

                    const auto binary = HexStringToBinString(dataView);

                    if (binary.size() == rxNode->size)
                    {
                        std::copy(binary.cbegin(), binary.cend(), rxNode->data);
                        rxNode->transmitMs = millis();
                        uart.print("p2p rx 0\r\n");
                    }
                }
            }

            // タイムアウトで受信モードへ推移
            if (millis() - rxNode->transmitMs > 100)
            {
                uart.print("p2p rx 0\r\n");
            }
        }
    }

    /// @brief 16進数文字列から、バイナリへ変換
    /// @param view
    /// @return
    inline std::vector<uint8_t> TLM922S::HexStringToBinString(Udon::ArrayView<const char> view)
    {
        if (view.size() & 0b1)
        {
            return {};
        }

        std::vector<uint8_t> result;

        for (size_t i = 0; i < view.size(); i += 2)
        {
            uint8_t byte;
            if (sscanf(view.data() + i, "%2hhx", &byte) == 1)
            {
                result.push_back(byte);
            }
            else
            {
                return {};
            }
        }

        return result;
    }
}    // namespace Udon
