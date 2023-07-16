//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    CANコントローラー経由 CANバス管理クラス
//
//-------------------------------------------------------------------

#pragma once

// #include <mcp2515.h>    // https://github.com/autowp/arduino-mcp2515

// #include <Udon/Algorithm/RingBuffer.hpp>
// #    include <Udon/Com/Can/CanInfo.hpp>
// #include <Udon/Com/Can/CanUtility.hpp>
// #include <Udon/Com/Can/ICanBus.hpp>

// #pragma once

// #include <mcp2515.h>    // https://github.com/autowp/arduino-mcp2515

// #include <Udon/Stl/list.hpp>    // Udon::list

// #include <Udon/Com/Can/CanBusInterface.hpp>
// #include <Udon/Com/Can/CanInfo.hpp>

// namespace Udon
// {

//     template <uint8_t CS, uint8_t Interrupt>
//     class CanBusSpi : public CanBusInterface
//     {

//         MCP2515 bus;

//         using DataLine = Udon::list<CanNodeInfo*>;
//         DataLine        tx;
//         DataLine        rx;
//         CanBusErrorInfo error;

//         static CanBusSpi* self;

//     public:
//         /// @brief constructor
//         /// @param {spi} SPIインスタンス
//         /// @param {spiClock} SPI動作クロック
//         /// @remark 動作クロックは CPUクロック/2 が最大値
//         CanBusSpi(SPIClass& spi, uint32_t spiClock = 10000000)
//             : bus(CS, spiClock, &spi)
//             , tx()
//             , rx()
//         {
//             self = this;
//         }

//         ~CanBusSpi()
//         {
//         }

//         /// @brief 通信を開始
//         /// @param {baudrate} Can通信レート
//         /// @param {CanClock} Can通信クロック
//         void begin(CAN_CLOCK canClock = MCP_20MHZ, CAN_SPEED baudrate = CAN_1000KBPS)
//         {
//             if (rx.size() || tx.size())
//             {
//                 bus.reset();
//                 bus.setBitrate(baudrate, canClock);
//                 bus.setNormalMode();
//             }
//             if (rx.size())
//             {
//                 pinMode(Interrupt, INPUT_PULLUP);
//                 attachInterrupt(
//                     digitalPinToInterrupt(Interrupt), []
//                     {
//                     can_frame msg;
//                     if (self->bus.readMessage(&msg) == MCP2515::ERROR_OK) {
//                         const auto event = [&msg](CanNodeInfo* node)
//                         {
//                             // 先頭1バイト : パケット番号
//                             const uint8_t index = msg.data[0];

//                             // 8バイト受信データをバイト列にデコード
//                             for (uint8_t i = 0; i < 7; i++)
//                             {
//                                 const uint8_t bufIndex = i + index * 7;
//                                 if (bufIndex < node->length)
//                                     node->buffer[bufIndex] = msg.data[i + 1];
//                                 else
//                                     break;
//                             }
//                             node->timestampUs = micros();
//                         };
//                         for (auto && it : self->rx) {
//                             if (msg.can_id == it->id)
//                             {
//                                 event(it);
//                             }
//                         }
//                         self->error.timestampUs = micros();
//                     } },
//                     CHANGE);
//             }
//         }

//         /// @brief バスを更新
//         /// @param {writeIntervalUs} 送信間隔
//         void update(uint32_t writeIntervalUs = 5000)
//         {
//             if (tx.size() && micros() - error.timestampUs >= writeIntervalUs)
//             {
//                 const auto event = [](CanNodeInfo* node)
//                 {
//                     // 一度に8バイトしか送れないため、パケットに分割し送信
//                     for (size_t index = 0; index < ceil(node->length / 7.0); index++)
//                     {
//                         can_frame msg;

//                         // 先頭1バイト : パケット番号
//                         msg.can_id  = node->id;
//                         msg.data[0] = index;

//                         // バイト列を8バイト受信データにエンコード
//                         for (uint8_t i = 0; i < 7; i++)
//                         {
//                             const uint8_t bufIndex = i + index * 7;

//                             if (bufIndex < node->length)
//                                 msg.data[i + 1] = node->buffer[bufIndex];
//                             else
//                                 break;
//                         }
//                         // バスに送信
//                         self->bus.sendMessage(&msg);
//                     }
//                     node->timestampUs = micros();
//                 };

//                 for (auto&& it : self->tx)
//                 {
//                     event(it);
//                 }
//                 error.timestampUs = micros();
//             }
//         }

//         CanBusErrorInfo getErrorInfo() const
//         {
//             return error;
//         }

//         void joinTX(CanNodeInfo& node) override
//         {
//             join(tx, node);
//         }
//         void joinRX(CanNodeInfo& node) override
//         {
//             join(rx, node);
//         }

//         void detachRX(CanNodeInfo& node) override
//         {
//             detach(rx, node);
//         }
//         void detachTX(CanNodeInfo& node) override
//         {
//             detach(tx, node);
//         }

//     private:
//         void join(DataLine& line, CanNodeInfo& node)
//         {
//             for (auto&& it : line)
//             {
//                 if (it == &node)    // インスタンスの重複を除外する
//                 {
//                     return;
//                 }
//             }
//             line.push_back(&node);
//         }

//         void detach(DataLine& line, CanNodeInfo& node)
//         {
//             for (auto&& it = line.begin(); it != line.end(); ++it)
//             {
//                 if (*it == &node)
//                 {
//                     line.erase(it);
//                     break;
//                 }
//             }
//         }
//     };

//     template <uint8_t CS, uint8_t Interrupt>
//     CanBusSpi<CS, Interrupt>* CanBusSpi<CS, Interrupt>::self;

// }    // namespace Udon
