/// @file   CanBusTeensy.hpp
/// @date   2023/01/13
/// @brief  FlexCan_T4ライブラリを用いたCanバス管理クラス
/// @remark [CPU] <--Can[TX/RX]--> [Canトランシーバ] <--Can[H/L]--> [BUS]
/// @author 大河 祐介

#pragma once

//     vvv teensy3.2 vvv        vvv teensy3.5 vvv         vvv teensy3.6 vvv       vvv teensy4.0/4.1 vvv
#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)

#include <FlexCAN_T4.h>       // https://github.com/tonton81/FlexCan_T4.git
#include <IntervalTimer.h>    // https://github.com/loglow/IntervalTimer.git
#include "list.hpp"    // udon::std::list

#include "CanBusInterface.hpp"
#include "CanInfo.hpp"

/// @tparam {Bus} バス種類 (Can0,Can1,Can2,Can3)
template <CAN_DEV_TABLE Bus>
class CanBusTeensy
    : public CanBusInterface
{

    FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;
    IntervalTimer                             isr;

    using DataLine = udon::std::list<CanNodeInfo*>;
    DataLine        tx;
    DataLine        rx;
    CanBusErrorInfo error;

    static CanBusTeensy* self;

public:
    CanBusTeensy()
        : bus{}
        , tx{}
        , rx{}
        , error{}
    {
        self = this;
    }

    ~CanBusTeensy()
    {
        end();
    }

    /// @brief 通信開始
    /// @param {baudrate} 通信レート
    void begin(const uint32_t baudrate = 1000000)
    {
        if (rx.size() || tx.size())
        {
            bus.begin();
            bus.setBaudRate(baudrate);
            bus.enableFIFO();
            bus.enableFIFOInterrupt();
        }
        if (rx.size())
        {
            bus.onReceive(eventRX);
            isr.begin(
                []
                { self->bus.events(); },
                100);
        }
    }

    /// @brief 通信終了
    void end()
    {
        isr.end();
        bus.disableFIFO();
        bus.disableFIFOInterrupt();
    }

    /// @brief バス更新
    /// @param {writeIntervalUs} 送信間隔
    void update(uint32_t writeIntervalUs = 5000)
    {
        if (tx.size() && micros() - error.timestampUs >= writeIntervalUs)
        {
            eventTX();
        }
    }

    /// @brief エラー情報取得
    /// @return CanBusErrorInfo構造体インスタンス
    CanBusErrorInfo getErrorInfo() const
    {
        return error;
    }

    /// @brief TXノードをバスに参加
    /// @param node
    void joinTX(CanNodeInfo& node) override
    {
        join(tx, node);
    }

    /// @brief RXノードをバスに参加
    /// @param node
    void joinRX(CanNodeInfo& node) override
    {
        join(rx, node);
    }

    /// @brief RXノードをバスから解放
    /// @param node
    void detachRX(CanNodeInfo& node) override
    {
        detach(rx, node);
    }

    /// @brief TXノードをバスから解放
    /// @param node
    void detachTX(CanNodeInfo& node) override
    {
        detach(tx, node);
    }

private:
    /// @brief ライブラリクラスからバスのエラー情報を取得
    static CanBusErrorInfo getError()
    {
        CAN_error_t e;
        self->bus.error(e, false);
        return {
            e.TX_ERR_COUNTER,
            e.RX_ERR_COUNTER,
            micros()
        };
    }

    static void eventRX(const CAN_message_t& msg)
    {
        const auto event = [&msg](CanNodeInfo* node)
        {
            const uint8_t index = msg.buf[0];    // 先頭1バイト : パケット番号
            for (uint8_t i = 0; i < 7; i++)      // 8バイト受信データをバイト列にデコード
            {
                const uint8_t bufIndex = i + index * 7;
                if (bufIndex < node->length)
                    node->buffer[bufIndex] = msg.buf[i + 1];
                else
                    break;
            }
            node->timestampUs = micros();
        };
        for (auto&& it : self->rx)
        {
            if (msg.id == it->id)
            {
                event(it);
            }
        }
        // self->error = getError();
    }

    static void eventTX()
    {
        const auto event = [](CanNodeInfo* node)
        {
            // 一度に8バイトしか送れないため、パケットに分割し送信
            for (size_t index = 0; index < ceil(node->length / 7.0); index++)
            {
                CAN_message_t msg;
                msg.id     = node->id;
                msg.buf[0] = index;                // 先頭1バイト : パケット番号
                for (uint8_t i = 0; i < 7; i++)    // バイト列を8バイト受信データにエンコード
                {
                    const uint8_t bufIndex = i + index * 7;
                    if (bufIndex < node->length)
                        msg.buf[i + 1] = node->buffer[bufIndex];
                    else
                        break;
                }
                while (not self->bus.write(msg))
                    ;    // 送信
            }
            node->timestampUs = micros();
        };
        for (auto&& it : self->tx)
        {
            event(it);
        }
        // self->error = getError();
    }

    void join(DataLine& line, CanNodeInfo& node)
    {
        for (auto&& it : line)
        {
            if (it == &node)    // インスタンスの重複を除外する
            {
                return;
            }
        }
        line.push_back(&node);
    }

    void detach(DataLine& line, CanNodeInfo& node)
    {
        for (auto&& it = line.begin(); it != line.end(); ++it)
        {
            if (*it == &node)
            {
                line.erase(it);
                break;
            }
        }
    }
};

template <CAN_DEV_TABLE Bus>
CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;

#endif
