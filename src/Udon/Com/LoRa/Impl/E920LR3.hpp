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

    inline bool E920LR3::begin(const E920Config& e920config, const bool setMode)
    {
        uart.begin(115200);    // パラメータセット時のビットレートは固定されている。

        if (INTPin)
        {
            pinMode(*INTPin, INPUT);
        }

        uart.setTimeout(10);
        // configMode パラメータセットモード

        delay(50);

        uart.write("2\r\n");
        waitResponse();

        // setParameter("protocol", e920config.protocolMode+48);
        // uart.write("protocol 3\r\n");
        setParameter("node", e920config.nodeType);
        setParameter("channel", e920config.channel);
        setParameter("panid", e920config.panid);
        setParameter("ownid", e920config.ownid);
        setParameter("dstid", e920config.dstid);
        setParameter("bw", e920config.bw);
        setParameter("sf", e920config.sf);


        // if (setMode)
        // {

        // }

        // // FSK用------------------
        // uart.write("channnel ");
        // uart.write(channel);
        // uart.write("\r\n");
        // waitResponse();
        // uart.write("panid ");
        // uart.write(panid);
        // uart.write("\r\n");
        // waitResponse();
        // uart.write("ownid ");
        // uart.write(myNode);
        // uart.write("\r\n");
        // waitResponse();
        // uart.write("dstid ");
        // uart.write(targetid);
        // uart.write("\r\n");
        // waitResponse();
        // uart.write("hopcount 1\r\n");
        // waitResponse();
        // uart.write("endid ");
        // uart.write(targetid);
        // uart.write("\r\n");
        // waitResponse();
        // uart.write("rate ");
        // uart.write(rate);
        // uart.write("\r\n");
        // waitResponse();


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
    inline E920LR3::TransmitMode E920LR3::getTransmitMode() const
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

    inline void E920LR3::setParameter(String parameter, uint16_t value)
    {
        uart.print(parameter);
        uart.write(" ");
        uart.write(value + 48);
        uart.write("\r\n");
        waitResponse();
    }

    inline bool E920LR3::sendUpdate()
    {
        if (millis() - txNode->transmitMs > 10)
        {
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
                    uart.write("\r\n");
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
            // Serial.println (uart.available());
            return false;
        }
        // Serial.println ("uart.available() >= FrameSize");
        // データ読み込み
        const auto hexString = uart.readStringUntil('\r');
        uart.read();    // \nを読み飛ばす

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
            Serial.println(millis() - rxNode->transmitMs);
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
        // while (startTime - millis() < 50)
        // {

        //     //     if (uart.available() > 3)
        //     //     {
        //     //         if (uart.read() == 'O' && uart.read() == 'K')
        //     //         {
        //     //             while (uart.available())
        //     //                 uart.read();
        //     //             return true;
        //     //         }
        //     //         else
        //     //         {
        //     //             while (uart.available())
        //     //                 uart.read();
        //     //             Serial.print("Configration Error\n");
        //     //             return false;
        //     //         }
        //     // }
        // }
        delay(100);
        while (uart.available() > 0)
        {
            Serial.write(uart.read());
        }
        return false;
    }
}    // namespace Udon
