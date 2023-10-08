#pragma once

#include "ILora.hpp"

#include <Udon/Algorithm/BitPack.hpp>
#include <Udon/Stl/Optional.hpp>
#include <Udon/Common/Printf.hpp>

namespace Udon
{
    class E220
        : public ILora
    {
        HardwareSerial& uart;

        LoraNode* txNode;
        LoraNode* rxNode;

        uint32_t lastWaitUntilCommandAcceptMs;

        Udon::Optional<uint8_t> M0Pin;
        Udon::Optional<uint8_t> M1Pin;
        Udon::Optional<uint8_t> AUXPin;
        uint8_t                 targetADDH;
        uint8_t                 targetADDL;
        uint8_t                 targetChannel;

    public:
        E220(HardwareSerial& uart, uint8_t M0Pin, uint8_t M1Pin, uint8_t AUXPin, uint8_t targetADDH, uint8_t targetADDL, uint8_t targetChannel)
            : uart(uart)
            , lastWaitUntilCommandAcceptMs()
            , M0Pin(M0Pin)
            , M1Pin(M1Pin)
            , AUXPin(AUXPin)
            , targetADDH(targetADDH)
            , targetADDL(targetADDL)
            , targetChannel(targetChannel)
        {
        }

        /// @brief Loraが使用可能かどうか
        /// @return Loraが使用可能ならtrue
        operator bool() const override { return uart && not isTimeout(700); }

        /// @brief 通信開始
        /// @param ADDH 上位アドレス
        /// @param ADDL 下位アドレス
        /// @param channel チャンネル番号
        bool begin(uint8_t ADDH, uint8_t ADDL, uint8_t channel);

        void update();

        /// @brief バスの状態を表示
        void show() const
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

        /// @brief 送信ノードを登録
        /// @param node
        void joinTx(LoraNode& node) override { txNode = &node; }

        /// @brief 受信ノードを登録
        void joinRx(LoraNode& node) override { rxNode = &node; }

        static int ClampChannel(int channel)
        {
            return constrain(channel, 0, 30);
        }

    private:
        enum class TransmitMode
        {
            Send,
            Receive,
            TwoWay,
            Empty,
        };

        /// @brief 通信モードを取得する
        TransmitMode getTransmitMode() const
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

        bool sendUpdate()
        {
            //送信休止時間分間隔をあける
            if (millis() - txNode->transmitMs > 50)
            {
                uart.write(targetADDH);
                uart.write(targetADDL);
                uart.write(targetChannel);
                Udon::BitPack(txNode->data, txNode->data + txNode->size, [this](uint8_t data)
                              { uart.write(data); });
                txNode->transmitMs = millis();
            }
            else
            {
                // 送信間隔が短すぎる場合は送信しない
                return false;
            }
            return true;
        }

        bool receiveUpdate()
        {
            // FrameSize = data
            const int FrameSize = Udon::BitPackedSize(rxNode->size);

            // 受信バッファにデータがない場合は何もしない
            if (uart.available() < FrameSize)
            {
                return false;
            }

            while (uart.peek() >> 7 != 1)
            {
                uart.read();
                if (uart.available() < FrameSize)
                {
                    return false;
                }
            }

            // データ読み込み
            if (not Udon::BitUnpack(rxNode->data, rxNode->data + rxNode->size, [this]() -> uint8_t
                                    { return uart.read(); }))
            {
                // 先頭バイトの  MSB が 1 でない場合はデータが壊れているので読み捨て
                while (uart.available())
                {
                    uart.read();
                }
                return false;
            }

            rxNode->transmitMs = millis();
            return true;
        }

        void twoWayUpdate()
        {
        }

        bool isTimeout(uint32_t timeoutMs) const
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

        void waitUntilCommandAccept()
        {
            if (AUXPin)
            {
                lastWaitUntilCommandAcceptMs = millis();

                while (!digitalRead(*AUXPin))    // AUXPinがLOWの間はコマンドを受け付けない
                {
                    delayMicroseconds(10);    // チャタリング防止

                    if (millis() - lastWaitUntilCommandAcceptMs > 200)
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
    };
    inline bool E220::begin(uint8_t ADDH, uint8_t ADDL, uint8_t channel)
    {
        uart.begin(9600);
        // パラメータセット時のビットレートは固定されている。
        if (AUXPin)
            pinMode(*AUXPin, INPUT);
        if (M0Pin)
            pinMode(*M0Pin, OUTPUT);
        if (M1Pin)
            pinMode(*M1Pin, OUTPUT);

        uart.setTimeout(10);

        waitUntilCommandAccept();
        digitalWrite(*M0Pin, HIGH);    // M0をHIGHに設定
        digitalWrite(*M1Pin, HIGH);    // M1をHIGHに設定
                                       // configMode パラメータセットモード

        waitUntilCommandAccept();
        uart.write((uint8_t)0xC1);
        uart.write((uint8_t)0x00);
        uart.write((uint8_t)0x08);

        delay(500);
        uart.read();    // 0xC1
        uart.read();    // 0x00
        uart.read();    // 0x08

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
            Serial.print("ADDH     :");
            Serial.println(uart.read(), HEX);
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
            Serial.print("ADDL     :");
            Serial.println(uart.read(), HEX);
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
        digitalWrite(*M0Pin, LOW);
        digitalWrite(*M1Pin, LOW);

        waitUntilCommandAccept();
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
}    // namespace Udon