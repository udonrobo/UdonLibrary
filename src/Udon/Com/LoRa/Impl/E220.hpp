//
//    E220 LoRaモジュール ドライバクラス実装部
//
//	  Copyright (c) 2022-2023 Fujimoto Ryo
//    Copyright (c) 2022-2023 Udonrobo
//

#pragma once

#include <Udon/Algorithm/HexStringParser.hpp>

namespace Udon
{
    inline E220::E220(HardwareSerial& uart, uint8_t M0Pin, uint8_t M1Pin)
        : uart(uart)
        , M0Pin(M0Pin)
        , M1Pin(M1Pin)
        , AUXPin(Udon::nullopt)
    {
    }

    inline E220::E220(HardwareSerial& uart, uint8_t M0Pin, uint8_t M1Pin, uint8_t AUXPin)
        : uart(uart)
        , M0Pin(M0Pin)
        , M1Pin(M1Pin)
        , AUXPin(AUXPin)
    {
    }

    inline bool E220::begin(const uint8_t channel, const uint16_t myAddress, const uint16_t targetAddress)
    {
        this->channel       = channel;
        this->targetAddress = targetAddress;

        uart.begin(9600);    // パラメータセット時のビットレートは固定されている。

        if (AUXPin)
        {
            pinMode(*AUXPin, INPUT);
        }
        pinMode(M0Pin, OUTPUT);
        pinMode(M1Pin, OUTPUT);

        uart.setTimeout(10);

        waitUntilCommandAccept();
        digitalWrite(M0Pin, HIGH);    // M0をHIGHに設定
        digitalWrite(M1Pin, HIGH);    // M1をHIGHに設定
                                      // configMode パラメータセットモード

        waitUntilCommandAccept();
        uart.write((uint8_t)0xC1);
        uart.write((uint8_t)0x00);
        uart.write((uint8_t)0x08);

        delay(500);
        uart.read();    // 0xC1
        uart.read();    // 0x00
        uart.read();    // 0x08

        const uint8_t ADDH = (myAddress >> 8) & 0xFF;
        const uint8_t ADDL = (myAddress >> 0) & 0xFF;

        const uint8_t defADDH      = uart.read();    // ADDH
        const uint8_t defADDL      = uart.read();    // ADDL
        const uint8_t defRate      = uart.read();    // uartRate(7,6,5) & airRate(4,3,2,1,0)
        const uint8_t defPower     = uart.read();    // subPacket(7,6) & RSSI noise(5) & notUsed(4,3,2) & SendPower(1,0)
        const uint8_t defChannel   = uart.read();    // channel
        const uint8_t defTransMode = uart.read();    // add RSSIByte(7) & transmittionMode(6) & notUsed(5,4,3) & worCycle(2,1,0)
        uart.read();                                 // KeyHighByte
        uart.read();                                 // KeyLowByte
        uart.read();                                 // Version

        const uint8_t uartRate  = defRate >> 5;
        const uint8_t airRate   = defRate && 0b00011111;
        const uint8_t subPacket = defPower >> 6;
        const uint8_t RSSInoise = (defPower >> 5) && 0b00000001;
        const uint8_t sendPower = defPower && 0b00000011;
        const uint8_t RSSIByte  = defTransMode >> 7;
        const uint8_t transMode = (defTransMode >> 6) && 0b00000001;
        const uint8_t worCycle  = defTransMode && 0b00000111;
        // 受信完了
        // ＜"C0"とか"C3"＞＋<開始アドレス>＋＜長さ＞＋<パラメータ>
        Serial.println("defaultParameters-----------------");
        Serial.print("ADDH     :");
        Serial.println(defADDH);
        Serial.print("ADDL     :");
        Serial.println(defADDL);
        Serial.print("channel  :");
        Serial.println(defChannel);
        Serial.print("uartRate :");
        Serial.println(uartRate);
        Serial.print("airRate  :");
        Serial.println(airRate);
        Serial.print("subPacket:");
        Serial.println(subPacket);
        Serial.print("sendPower:");
        Serial.println(sendPower);
        Serial.print("RSSInoise:");
        Serial.println(RSSInoise ? "add" : "not add");
        Serial.print("add RSSIByte :");
        Serial.println(RSSIByte ? "add" : "not add");
        Serial.print("TransMode:");
        Serial.println(transMode ? "FixedTransmissionMode" : "TransparentMode");
        Serial.print("WorCycle:");
        Serial.println(worCycle);
        Serial.println("---------------------------------");

        Serial.println("setParameters-----------------");

        if (ADDH != defADDH)
        {
            waitUntilCommandAccept();
            uart.write((uint8_t)0xC2);
            uart.write((uint8_t)0x00);
            uart.write((uint8_t)0x01);
            uart.write(ADDH);
            delay(200);
            uart.read();    // 0xC1
            uart.read();    // 0x00
            uart.read();    // 0x01
        }

        if (ADDL != defADDL)
        {
            waitUntilCommandAccept();
            uart.write((uint8_t)0xC2);
            uart.write((uint8_t)0x01);
            uart.write((uint8_t)0x01);
            uart.write(ADDL);
            delay(200);
            uart.read();    // 0xC1
            uart.read();    // 0x01
            uart.read();    // 0x01
        }

        if (ADDH != defADDH or ADDL != defADDL)
        {
            Serial.print("Address :");
            Serial.print(myAddress);
        }

        if (defRate != 0b11100010)
        {
            waitUntilCommandAccept();
            uart.write((uint8_t)0xC2);
            uart.write((uint8_t)0x02);
            uart.write((uint8_t)0x01);
            uart.write((uint8_t)0b11100010);
            delay(200);
            uart.read();    // 0xC1
            uart.read();    // 0x02
            uart.read();    // 0x01
            Serial.print("Uart & AirRate:");
            Serial.println(uart.read(), BIN);
        }

        if (defPower != 0b00000001)
        {
            waitUntilCommandAccept();
            uart.write((uint8_t)0xC2);
            uart.write((uint8_t)0x03);
            uart.write((uint8_t)0x01);
            uart.write((uint8_t)0b00000001);
            delay(200);
            uart.read();    // 0xC1
            uart.read();    // 0x03
            uart.read();    // 0x01
            Serial.print("SubPacket & RSSINoise & SendPower:");
            Serial.println(uart.read(), BIN);
        }

        if (channel != defChannel)
        {
            waitUntilCommandAccept();
            uart.write((uint8_t)0xC2);
            uart.write((uint8_t)0x04);
            uart.write((uint8_t)0x01);
            uart.write(channel);
            delay(200);
            uart.read();    // 0xC1
            uart.read();    // 0x02
            uart.read();    // 0x01
            Serial.print("Channel :");
            Serial.println(uart.read());
        }

        if (transMode != 0b01000011)
        {
            waitUntilCommandAccept();
            uart.write((uint8_t)0xC2);
            uart.write((uint8_t)0x05);
            uart.write((uint8_t)0x01);
            uart.write((uint8_t)0b01000011);
            delay(200);
            uart.read();    // 0xC1
            uart.read();    // 0x05
            uart.read();    // 0x01
            Serial.print("RSSIByte & TransMode & WORCycle:");
            Serial.println(uart.read(), BIN);
        }
        Serial.println("---------------------------------");

        // 設定モードの終了
        waitUntilCommandAccept();
        digitalWrite(M0Pin, LOW);
        digitalWrite(M1Pin, LOW);

        uart.end();
        uart.begin(115200);    // 通常通信モードのレートに変更する
        uart.setTimeout(10);
        return true;
    }

    inline void E220::update()
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

    inline void E220::show() const
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
    inline E220::TransmitMode E220::getTransmitMode() const
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

    inline bool E220::sendUpdate()
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

    inline bool E220::receiveUpdate()
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

    inline void E220::twoWayUpdate()
    {
    }

    inline bool E220::isTimeout(uint32_t timeoutMs) const
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

    inline void E220::waitUntilCommandAccept()
    {
        if (AUXPin)
        {
            const auto lastWaitMs = millis();

            while (!digitalRead(*AUXPin))    // AUXPinがLOWの間はコマンドを受け付けない
            {
                delayMicroseconds(10);    // チャタリング防止

                if (millis() - lastWaitMs > 200)
                {
                    break;    // タイムアウトした場合は強制的にコマンドを受け付ける
                }
            }
            delay(3);    // mode切り替え指示後2~3ms秒後に切り替わる
        }
        else
        {
            delay(200);    // AUXPin が設定されていない場合は200ms待つ
        }

        // 待機している間に余計なデータが送られてきている可能性があるので読み捨てる
        while (uart.available())
        {
            uart.read();
        }
    }
}    // namespace Udon
