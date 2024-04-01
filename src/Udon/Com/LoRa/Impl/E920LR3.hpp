//
//    E920 LoRaモジュール ドライバクラス実装部
//
//	  Copyright (c) 2022-2023 Fujimoto Ryo
//    Copyright (c) 2022-2023 Udonrobo
//

#pragma once

#include <Udon/Algorithm/HexStringParser.hpp>

namespace Udon
{
    inline E920LR3::E920LR3(HardwareSerial& uart)
        : uart(uart)
        , INTPin(Udon::nullopt)
    {
    }

    inline E920LR3::E920LR3(HardwareSerial& uart, uint8_t INTPin)
        : uart(uart)
        , INTPin(INTPin)
    {
    }

    inline bool E920LR3::begin(Config& config)
    {
        uart.begin(115200);    // パラメータセット時のビットレートは固定されている。

        if (INTPin)
        {
            pinMode(*INTPin, INPUT);
        }

        uart.setTimeout(10);
        // configMode パラメータセットモード

        delay(50);
        uart.write(2);
        uart.write("\r\n");
        waitResponse();


        uart.write("protocol 3\r\n");
        waitResponse();
        // LoRa用------------------
        uart.write("channnel ");
        uart.write(channel);
        uart.write("\r\n");
        waitResponse();
        uart.write("bw 5\r\n");
        waitResponse();
        uart.write("sf 5\r\n");
        waitResponse();
        uart.write("panid ");
        uart.write(panid);
        uart.write("\r\n");
        waitResponse();
        uart.write("ownid ");
        uart.write(myNode);
        uart.write("\r\n");
        waitResponse();
        uart.write("dstid ");
        uart.write(targetid);
        uart.write("\r\n");
        waitResponse();
        uart.write("hopcount 1\r\n");
        waitResponse();
        // FSK用------------------
        uart.write("channnel ");
        uart.write(channel);
        uart.write("\r\n");
        waitResponse();
        uart.write("panid ");
        uart.write(panid);
        uart.write("\r\n");
        waitResponse();
        uart.write("ownid ");
        uart.write(myNode);
        uart.write("\r\n");
        waitResponse();
        uart.write("dstid ");
        uart.write(targetid);
        uart.write("\r\n");
        waitResponse();
        uart.write("hopcount 1\r\n");
        waitResponse();
        uart.write("endid ");
        uart.write(targetid);
        uart.write("\r\n");
        waitResponse();
        uart.write("rate ");
        uart.write(rate);
        uart.write("\r\n");
        waitResponse();

        //------------------
        // 一度きりの設定
        uart.write("ack 2\r\n");
        waitResponse();
        uart.write("rflpmode 3\r\n");
        waitResponse();


        //------------------
        uart.write("save\r\n");
        waitResponse();
        uart.write("start\r\n");
        waitResponse();

        uart.setTimeout(10);
        return true;
    }

    inline void E920LR3::update()
    {
        switch (getTransmitMode())
        {
        case TransmitMode::Send:
            sendUpdate();
            break;
        case TransmitMode::Receive:
            receiveUpdate();
            break;
        case TransmitMode::TwoWay:
            twoWayUpdate();
            break;
        case TransmitMode::Empty:
            break;
        }
    }

    inline void E920LR3::show() const
    {
        if (operator bool())
        {
            Serial.print("E220: OK  ");
        }
        else
        {
            Serial.print("E220: NG  ");
        }

        switch (getTransmitMode())
        {
        case TransmitMode::Send: Serial.print("SendMode "); break;
        case TransmitMode::Receive: Serial.print("ReceiveMode "); break;
        case TransmitMode::TwoWay: Serial.print("TwoWayMode "); break;
        case TransmitMode::Empty: Serial.print("Empty "); break;
        }
    }

    /// @brief 通信モードを取得する
    inline E920LR3::TransmitMode E220::getTransmitMode() const
    {
        if (txNode)
        {
            if (rxNode)
            {
                return TransmitMode::TwoWay;
            }
            else
            {
                return TransmitMode::Send;
            }
        }
        else if (rxNode)
        {
            return TransmitMode::Receive;
        }
        else
        {
            return TransmitMode::Empty;
        }
    }

    inline bool E920LR3::sendUpdate()
    {
        // 送信休止時間分間隔をあける
        if (millis() - txNode->transmitMs > 50)
        {
            // ヘッダー送信
            {
                const uint8_t highAddress = (targetAddress >> 8) & 0xFF;
                const uint8_t lowAddress  = (targetAddress >> 0) & 0xFF;
                uart.write(highAddress);
                uart.write(lowAddress);
                uart.write(channel);
            }

            // データ送信
            {
                std::vector<char> hexString;
                hexString.resize(Udon::ConvertedByteStringSize(txNode->size));

                const auto byteStringView = Udon::ArrayView<const uint8_t>{ txNode->data, txNode->size };
                const auto hexStringView  = Udon::ArrayView<char>{ hexString.data(), hexString.size() };

                if (Udon::ByteStringToHexString(byteStringView, hexStringView))
                {
                    for (const auto& hex : hexStringView)
                    {
                        uart.write(hex);
                    }
                    uart.write('\n');

                    txNode->transmitMs = millis();
                }
            }
        }
        else
        {
            // 送信間隔が短すぎる場合は送信しない
            return false;
        }
        return true;
    }

    inline bool E920LR3::receiveUpdate()
    {

        // 16進数文字列が送られてくるので、そのサイズを求める
        const int FrameSize = Udon::ConvertedByteStringSize(rxNode->size);

        // 受信バッファにデータがない場合は何もしない
        if (uart.available() < FrameSize)
        {
            // Serial.println ("uart.available() < FrameSize");
            return false;
        }
        // Serial.println ("uart.available() >= FrameSize");

        // データ読み込み
        const auto hexString = uart.readStringUntil('\n');

        // Serial.println(hexString);
        if (hexString.length() == 0)
        {
            return false;
        }

        // 16進数文字列をバイト列に変換する
        ArrayView<const char> hexStringView{ hexString.c_str(), hexString.length() };
        ArrayView<uint8_t>    byteStringView{ rxNode->data, rxNode->size };
        if (Udon::HexStringToByteString(hexStringView, byteStringView))
        {
            rxNode->transmitMs = millis();
        }
        else
        {
            return false;
        }
        return true;
    }

    inline void E920LR3::twoWayUpdate()
    {
    }

    inline bool E920LR3::isTimeout(uint32_t timeoutMs) const
    {
        switch (getTransmitMode())
        {
        case TransmitMode::Send:
            return millis() - txNode->transmitMs > timeoutMs;
        case TransmitMode::Receive:
            return millis() - rxNode->transmitMs > timeoutMs;
        case TransmitMode::TwoWay:
            return millis() - txNode->transmitMs > timeoutMs || millis() - rxNode->transmitMs > timeoutMs;
        case TransmitMode::Empty:
            return false;
        default:
            return false;
        }
    }

    inline bool E920LR3::waitResponse()
    {
        // Response(ASCII)  [response] + [半角スペース(option)] + [resposeCode(option)] + [\r\n]
        uint32_t startTime = millis();
        while (startTime - millis() < 50)
        {
            if (uart.available() > 3)
            {
                if (uart.read() == "O" && uart.read() == "K")
                {
                    while (uart.available())
                        uart.read();
                    return true;
                }
                else
                {
                    while (uart.available())
                        uart.read();
                    Serial.print("Configration Error\n");
                    return false;
                }
            }
        }
        while (uart.available())
            uart.read();
        Serial.print("Configration Error\n");
        return false;
    }
}    // namespace Udon
