#pragma once

#include <Udon/Algorithm/ArrayView.hpp>

#include <algorithm>
#include <cmath>

namespace Udon
{
    /// @brief 複数のパケット、単一のパケットからバイト列にアンパケット化する
    /// @details
    ///        出力バッファ(output)のサイズによってアンパケット方法が変わります。
    ///        0 < output.size <= singlePacketSize:  シングルパケットとしてアンパケット化(コピー)
    ///        singlePacketSize < output.size     :  マルチパケット  としてアンパケット化(先頭バイトをパケットインデックスとしてコピー)
    /// @param input 入力パケット[何番目かのパケット]
    /// @param output アンパケット化したデータの格納先
    /// @param singlePacketSize 1パケットのサイズ
    inline void Unpacketize(
        Udon::ArrayView<uint8_t>&& input,              // _In_
        Udon::ArrayView<uint8_t>&& output,             // _Out_
        size_t                     singlePacketSize    // _In_
    )
    {

        if (singlePacketSize == 0)
        {
            return;
        }

        if (output.size() <= singlePacketSize)
        {
            // single packetize
            // packet: [data][data]...[data] <=8byte
            std::copy_n(
                input.begin(),
                std::min(input.size(), output.size()),
                output.begin());
        }
        else
        {
            // multi packetize
            // packet: [index][data][data]...[data] <=8byte

            const auto index = input.at(0);

            const size_t dataSize    = singlePacketSize - 1;    // -1: index
            const size_t packetCount = static_cast<size_t>(std::ceil(
                static_cast<double>(output.size()) / dataSize));

            if (index >= packetCount)
            {
                // invalid index (buffer overflow)
                return;
            }
            if (index == packetCount - 1)
            {
                // last packet (copy remain data)
                std::copy_n(
                    input.begin() + 1,    // skip index
                    output.size() - index * dataSize,
                    output.begin() + index * dataSize);
                return;
            }
            else
            {
                std::copy_n(
                    input.begin() + 1,    // skip index
                    dataSize,
                    output.begin() + index * dataSize);
            }
        }
    }

    /// @brief バイト列を複数のパケット、単一のパケットにパケット化する
    /// @details
    ///        入力バッファ(input)のサイズによってアンパケット方法が変わります。
    ///        0 < input.size <= singlePacketSize:  シングルパケットとしてアンパケット化(コピー)
    ///        singlePacketSize < input.size     :  マルチパケット  としてアンパケット化(先頭バイトをパケットインデックスとしてコピー)
    /// @tparam Function
    /// @param input バイト列
    /// @param output パケット化したデータの格納先
    /// @param singlePacketSize 1パケットのサイズ
    /// @param func パケット化した後に呼び出される関数オブジェクト(引数 size_t: パケットに含まれるデータサイズ)
    template <typename Function>
    inline void Packetize(
        Udon::ArrayView<uint8_t>&& input,               // _In_
        Udon::ArrayView<uint8_t>&& output,              // _Out_
        size_t                     singlePacketSize,    // _In_
        Function                   func                 // _In_
    )
    {

        if (input.size() > singlePacketSize)
        {
            // multi packetize
            // packet: [index][data][data]...[data] <=8byte
            const size_t dataSize    = singlePacketSize - 1;
            const size_t packetCount = static_cast<size_t>(std::ceil(
                static_cast<double>(input.size()) / dataSize));

            for (uint8_t index = 0; index < packetCount; ++index)
            {
                output.at(0) = index;

                if (index == packetCount - 1)
                {
                    // last packet
                    std::copy_n(
                        input.begin() + index * dataSize,
                        std::min(dataSize, input.size() - index * dataSize),
                        output.begin() + 1);
                    func(input.size() - index * dataSize);    // callback
                }
                else
                {
                    std::copy_n(
                        input.begin() + index * dataSize,
                        dataSize,
                        output.begin() + 1);
                    func(singlePacketSize);    // callback
                }
            }
        }
        else
        {
            // single packetize
            // packet: [data][data]...[data] <=8byte
            std::copy_n(
                input.begin(),
                input.size(),
                output.begin());
            func(input.size());    // callback
        }
    }

#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__) || defined(ARDUINO_ARCH_RP2040)

    /// @brief バスの情報をプリントする
    /// @tparam Nodes
    /// @param busName
    /// @param txNodes
    /// @param rxNodes
    template <typename TxNodesBuffer, typename RxNodesBuffer>
    inline void PrintBusInfo(
        const char*          busName,            // _In_
        const TxNodesBuffer& txNodes,            // _In_
        const RxNodesBuffer& rxNodes,            // _In_
        size_t               singlePacketSize    // _In_
    )
    {
        Serial.printf("Bus: %s\n", busName);

        Serial.printf("\tTX Node\n");
        for (auto node = txNodes.cbegin(); node != txNodes.cend(); ++node)
        {
            Serial.printf("\t\tid:%4d   size:%3zu byte", node->id, node->data.size());
            if (node->data.size() > singlePacketSize)
            {
                Serial.printf(" (multi packet)");
            }
            else
            {
                Serial.printf(" (single packet)");
            }

            Serial.printf("\n\t\t\tdata: ");
            for (auto&& data : node->data)
            {
                Serial.printf("%4d", data);
            }

            Serial.printf("\n");
        }

        Serial.printf("\tRX Node\n");
        for (auto node = rxNodes.cbegin(); node != rxNodes.cend(); ++node)
        {
            Serial.printf("\t\tid:%4d   size:%3zu byte", node->id, node->data.size());
            if (node->data.size() > singlePacketSize)
            {
                Serial.printf(" (multi packet)");
            }
            else
            {
                Serial.printf(" (single packet)");
            }

            Serial.printf("\n\t\t\tdata: ");
            for (auto&& data : node->data)
            {
                Serial.printf("%4d", data);
            }

            Serial.printf("\n");
        }
    }

#endif
}    // namespace Udon
