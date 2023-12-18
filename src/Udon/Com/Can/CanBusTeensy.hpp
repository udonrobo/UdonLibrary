//
//    CAN通信 Teensy用バスクラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <FlexCAN_T4.h>    // https://github.com/tonton81/FlexCan_T4.git

#include "ICanBus.hpp"
#include "CanNode.hpp"
#include "CanUtility.hpp"

#include <Udon/Types/StaticVector.hpp>

namespace Udon
{

    /// @brief FlexCan_T4ライブラリを用いたCANバスクラス
    /// @tparam Bus バス種類
    /// @remark 使用するTeensyの種類によってCANバスの種類が異なるので注意(対応していないバスを指定するとstatic_assertが発生します)
    /// - Teensy 3.2: CAN0
    /// - Teensy 3.5: CAN0
    /// - Teensy 3.6: CAN0 & CAN1
    /// - Teensy 4.0: CAN1 & CAN2 & CAN3
    template <CAN_DEV_TABLE Bus>
    class CanBusTeensy
        : public ICanBus
    {
    public:
        struct CanConfig
        {
            uint32_t transmitInterval = 5;            // 送信間隔 [ms]
            uint32_t transmitTimeout  = 100;          // 送信タイムアウト時間 [ms]
            uint32_t receiveTimeout   = 100;          // 受信タイムアウト時間 [ms]
            uint32_t baudrate         = 1'000'000;    // CAN通信速度 [bps]
        };

        /// @brief コンストラクタ
        /// @param config CAN設定情報 [optional]
        explicit CanBusTeensy(const CanConfig& config = {});

        /// @brief コピーコンストラクタ
        CanBusTeensy(const CanBusTeensy&);

        /// @brief デストラクタ
        ~CanBusTeensy();

        /// @brief 通信開始
        /// @remark 呼び出し必須
        void begin();

        /// @brief 通信終了
        void end();

        /// @brief バス更新
        /// @remark 呼び出し必須
        void update();

        /// @brief バスが有効かどうか
        /// @remark 受信タイムアウトもしくは送信タイムアウトが発生した場合はfalseを返す
        /// @remark 全ての受信ノード送信ノードが有効であるかは考慮しない
        explicit operator bool() const override;

        /// @brief 送信タイムアウト
        bool txTimeout() const;

        /// @brief 受信タイムアウト
        bool rxTimeout() const;

        /// @brief バス情報を表示する
        void show() const;

        void joinTx(CanNode& node) override;

        void joinRx(CanNode& node, void (*onReceive)(void*), void* p) override;

        void leaveTx(const CanNode& node) override;

        void leaveRx(const CanNode& node) override;

    private:
        static CanBusTeensy* self;    // コールバック関数から自身のインスタンスを参照するためのポインタ (クラステンプレートによってインスタンスごとに別のstatic変数が生成される)

        CanConfig config;

        FlexCAN_T4<Bus, RX_SIZE_256, TX_SIZE_256> bus;

        constexpr static uint8_t SingleFrameSize = 8;

        using TxNodePtr = CanNode*;
        struct RxNodePtr
        {
            CanNode* node;
            void (*onReceive)(void*);
            void* p;

            void callback()
            {
                if (onReceive)
                {
                    onReceive(p);
                }
            }
        };

        Udon::StaticVector<TxNodePtr> txNodes;    // 送信ノード
        Udon::StaticVector<RxNodePtr> rxNodes;    // 受信ノード

        Udon::StaticVector<CAN_message_t, 1024> rxBuffer;    // 受信バッファ

        uint32_t transmitMs = 0;    // 最終送信成功時刻
        uint32_t receiveMs  = 0;    // 最終受信成功時刻

        /// @brief 受信処理
        void onReceive();

        /// @brief 送信処理
        void onTransmit();
    };

}    // namespace Udon

#include "Impl/CanBusTeensy.hpp"
