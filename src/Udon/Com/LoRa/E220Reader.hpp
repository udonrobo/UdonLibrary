//
//    E220 無線モジュール受信クラス
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Algorithm/ScopedInterruptLocker.hpp>
#include <Udon/Types/Optional.hpp>
#include "E220Base.hpp"

namespace Udon
{

    /// @brief E220 受信クラス
    /// @tparam T 受信するメッセージ型
    template <typename T>
    class E220Reader
        : public Impl::E220Base
    {
    public:
        /// @brief メッセージサイズ
        static constexpr size_t Size = Udon::SerializedSize<T>();

        /// @brief メッセージ型
        using MessageType = T;

        /// @brief コピーコンストラクタ
        /// @note コールバックをする際にthisを登録する必要があり、コピー元のオブジェクトが使用できなくなるため削除
        E220Reader(const E220Base::Config& config) = delete;

        /// @brief ムーブコンストラクタ
        /// @param config 設定
        E220Reader(E220Base::Config&& config)
            : E220Base(config)
        {
        }

        /// @brief 受信開始
        void begin() noexcept
        {
            E220Base::begin();
            delay(100);
            darkAttachInterrupt(config.aux);
        }

        /// @brief 受信開始
        /// @param channel
        void begin(uint8_t channel) noexcept
        {
            config.channel = channel;
            begin();
        }

        /// @brief RSSI 強度取得
        /// @note 受信エラー時の戻り値は不定
        /// @note RSSI 強度は最後に受信したメッセージのものを返す
        /// @return RSSI 強度
        int getRssi() const noexcept
        {
            ScopedInterruptLocker locker;

            if (rawRssi == 0)
            {
                // アンテナ同士が近すぎると 0 になる
                return 0;
            }
            else
            {
                return -(256 - rawRssi);
            }
        }

        /// @brief メッセージを受信
        /// @return メッセージ
        /// @note 受信エラー時、受信タイムアウト時は nullopt を返す
        /// @note メッセージは最後に受信したものを返す (受信エラー時は前回のメッセージを返す)
        Udon::Optional<MessageType> getMessage() noexcept
        {
            ScopedInterruptLocker locker;

            if (received)
            {
                lastReceiveMs = millis();    // 割り込み内でのmillis()は正確な値を取得できないため、このタイミングで取得
                received = false;
            }

            if (millis() - lastReceiveMs > 500)
            {
                // 受信タイムアウト
                return Udon::nullopt;
            }

            return Udon::Deserialize<MessageType>(buffer);
        }

    private:
        uint32_t lastReceiveMs = 0;

        // 排他制御必須 (割り込み関数内で使用)
        bool received = false;
        uint8_t buffer[Size];
        uint8_t rawRssi;

        void OnRisingEdge()
        {
            if (config.serial.available() == Size + 1 /* RSSIバイト*/)
            {
                config.serial.readBytes(buffer, sizeof buffer);
                rawRssi = config.serial.read();
                received = true;
            }
            else
            {
                Serial.println("E220Reader: Message size mismatch");
                while (config.serial.available())
                {
                    (void)config.serial.read();
                }
            }
        }

        static E220Reader** InstanceList()
        {
            static E220Reader* instanceList[60];
            return instanceList;
        }

        void darkAttachInterrupt(uint8_t pin)
        {
            // attachInterrupt は this ポインタの情報がないためメンバ関数を呼び出すことができない
            // そこでピン番号とthisポインタを紐づけて、無理やりメンバ関数を呼び出す

            InstanceList()[digitalPinToInterrupt(pin)] = this;

            // clang-format off
            switch (pin)
            {
            case  0: attachInterrupt(digitalPinToInterrupt( 0), [](){ InstanceList()[ 0]->OnRisingEdge(); }, RISING); break;
            case  1: attachInterrupt(digitalPinToInterrupt( 1), [](){ InstanceList()[ 1]->OnRisingEdge(); }, RISING); break;
            case  2: attachInterrupt(digitalPinToInterrupt( 2), [](){ InstanceList()[ 2]->OnRisingEdge(); }, RISING); break;
            case  3: attachInterrupt(digitalPinToInterrupt( 3), [](){ InstanceList()[ 3]->OnRisingEdge(); }, RISING); break;
            case  4: attachInterrupt(digitalPinToInterrupt( 4), [](){ InstanceList()[ 4]->OnRisingEdge(); }, RISING); break;
            case  5: attachInterrupt(digitalPinToInterrupt( 5), [](){ InstanceList()[ 5]->OnRisingEdge(); }, RISING); break;
            case  6: attachInterrupt(digitalPinToInterrupt( 6), [](){ InstanceList()[ 6]->OnRisingEdge(); }, RISING); break;
            case  7: attachInterrupt(digitalPinToInterrupt( 7), [](){ InstanceList()[ 7]->OnRisingEdge(); }, RISING); break;
            case  8: attachInterrupt(digitalPinToInterrupt( 8), [](){ InstanceList()[ 8]->OnRisingEdge(); }, RISING); break;
            case  9: attachInterrupt(digitalPinToInterrupt( 9), [](){ InstanceList()[ 9]->OnRisingEdge(); }, RISING); break;
            case 10: attachInterrupt(digitalPinToInterrupt(10), [](){ InstanceList()[10]->OnRisingEdge(); }, RISING); break;
            case 11: attachInterrupt(digitalPinToInterrupt(11), [](){ InstanceList()[11]->OnRisingEdge(); }, RISING); break;
            case 12: attachInterrupt(digitalPinToInterrupt(12), [](){ InstanceList()[12]->OnRisingEdge(); }, RISING); break;
            case 13: attachInterrupt(digitalPinToInterrupt(13), [](){ InstanceList()[13]->OnRisingEdge(); }, RISING); break;
            case 14: attachInterrupt(digitalPinToInterrupt(14), [](){ InstanceList()[14]->OnRisingEdge(); }, RISING); break;
            case 15: attachInterrupt(digitalPinToInterrupt(15), [](){ InstanceList()[15]->OnRisingEdge(); }, RISING); break;
            case 16: attachInterrupt(digitalPinToInterrupt(16), [](){ InstanceList()[16]->OnRisingEdge(); }, RISING); break;
            case 17: attachInterrupt(digitalPinToInterrupt(17), [](){ InstanceList()[17]->OnRisingEdge(); }, RISING); break;
            case 18: attachInterrupt(digitalPinToInterrupt(18), [](){ InstanceList()[18]->OnRisingEdge(); }, RISING); break;
            case 19: attachInterrupt(digitalPinToInterrupt(19), [](){ InstanceList()[19]->OnRisingEdge(); }, RISING); break;
            case 20: attachInterrupt(digitalPinToInterrupt(20), [](){ InstanceList()[20]->OnRisingEdge(); }, RISING); break;
            case 21: attachInterrupt(digitalPinToInterrupt(21), [](){ InstanceList()[21]->OnRisingEdge(); }, RISING); break;
            case 22: attachInterrupt(digitalPinToInterrupt(22), [](){ InstanceList()[22]->OnRisingEdge(); }, RISING); break;
            case 23: attachInterrupt(digitalPinToInterrupt(23), [](){ InstanceList()[23]->OnRisingEdge(); }, RISING); break;
            case 24: attachInterrupt(digitalPinToInterrupt(24), [](){ InstanceList()[24]->OnRisingEdge(); }, RISING); break;
            case 25: attachInterrupt(digitalPinToInterrupt(25), [](){ InstanceList()[25]->OnRisingEdge(); }, RISING); break;
            case 26: attachInterrupt(digitalPinToInterrupt(26), [](){ InstanceList()[26]->OnRisingEdge(); }, RISING); break;
            case 27: attachInterrupt(digitalPinToInterrupt(27), [](){ InstanceList()[27]->OnRisingEdge(); }, RISING); break;
            case 28: attachInterrupt(digitalPinToInterrupt(28), [](){ InstanceList()[28]->OnRisingEdge(); }, RISING); break;
            case 29: attachInterrupt(digitalPinToInterrupt(29), [](){ InstanceList()[29]->OnRisingEdge(); }, RISING); break;
            case 30: attachInterrupt(digitalPinToInterrupt(30), [](){ InstanceList()[30]->OnRisingEdge(); }, RISING); break;
            case 31: attachInterrupt(digitalPinToInterrupt(31), [](){ InstanceList()[31]->OnRisingEdge(); }, RISING); break;
            case 32: attachInterrupt(digitalPinToInterrupt(32), [](){ InstanceList()[32]->OnRisingEdge(); }, RISING); break;
            case 33: attachInterrupt(digitalPinToInterrupt(33), [](){ InstanceList()[33]->OnRisingEdge(); }, RISING); break;
            case 34: attachInterrupt(digitalPinToInterrupt(34), [](){ InstanceList()[34]->OnRisingEdge(); }, RISING); break;
            case 35: attachInterrupt(digitalPinToInterrupt(35), [](){ InstanceList()[35]->OnRisingEdge(); }, RISING); break;
            case 36: attachInterrupt(digitalPinToInterrupt(36), [](){ InstanceList()[36]->OnRisingEdge(); }, RISING); break;
            case 37: attachInterrupt(digitalPinToInterrupt(37), [](){ InstanceList()[37]->OnRisingEdge(); }, RISING); break;
            case 38: attachInterrupt(digitalPinToInterrupt(38), [](){ InstanceList()[38]->OnRisingEdge(); }, RISING); break;
            case 39: attachInterrupt(digitalPinToInterrupt(39), [](){ InstanceList()[39]->OnRisingEdge(); }, RISING); break;
            case 40: attachInterrupt(digitalPinToInterrupt(40), [](){ InstanceList()[40]->OnRisingEdge(); }, RISING); break;
            case 41: attachInterrupt(digitalPinToInterrupt(41), [](){ InstanceList()[41]->OnRisingEdge(); }, RISING); break;
            case 42: attachInterrupt(digitalPinToInterrupt(42), [](){ InstanceList()[42]->OnRisingEdge(); }, RISING); break;
            case 43: attachInterrupt(digitalPinToInterrupt(43), [](){ InstanceList()[43]->OnRisingEdge(); }, RISING); break;
            case 44: attachInterrupt(digitalPinToInterrupt(44), [](){ InstanceList()[44]->OnRisingEdge(); }, RISING); break;
            case 45: attachInterrupt(digitalPinToInterrupt(45), [](){ InstanceList()[45]->OnRisingEdge(); }, RISING); break;
            case 46: attachInterrupt(digitalPinToInterrupt(46), [](){ InstanceList()[46]->OnRisingEdge(); }, RISING); break;
            case 47: attachInterrupt(digitalPinToInterrupt(47), [](){ InstanceList()[47]->OnRisingEdge(); }, RISING); break;
            case 48: attachInterrupt(digitalPinToInterrupt(48), [](){ InstanceList()[48]->OnRisingEdge(); }, RISING); break;
            case 49: attachInterrupt(digitalPinToInterrupt(49), [](){ InstanceList()[49]->OnRisingEdge(); }, RISING); break;
            case 50: attachInterrupt(digitalPinToInterrupt(50), [](){ InstanceList()[50]->OnRisingEdge(); }, RISING); break;
            case 51: attachInterrupt(digitalPinToInterrupt(51), [](){ InstanceList()[51]->OnRisingEdge(); }, RISING); break;
            case 52: attachInterrupt(digitalPinToInterrupt(52), [](){ InstanceList()[52]->OnRisingEdge(); }, RISING); break;
            case 53: attachInterrupt(digitalPinToInterrupt(53), [](){ InstanceList()[53]->OnRisingEdge(); }, RISING); break;
            case 54: attachInterrupt(digitalPinToInterrupt(54), [](){ InstanceList()[54]->OnRisingEdge(); }, RISING); break;
            case 55: attachInterrupt(digitalPinToInterrupt(55), [](){ InstanceList()[55]->OnRisingEdge(); }, RISING); break;
            case 56: attachInterrupt(digitalPinToInterrupt(56), [](){ InstanceList()[56]->OnRisingEdge(); }, RISING); break;
            case 57: attachInterrupt(digitalPinToInterrupt(57), [](){ InstanceList()[57]->OnRisingEdge(); }, RISING); break;
            case 58: attachInterrupt(digitalPinToInterrupt(58), [](){ InstanceList()[58]->OnRisingEdge(); }, RISING); break;
            case 59: attachInterrupt(digitalPinToInterrupt(59), [](){ InstanceList()[59]->OnRisingEdge(); }, RISING); break;
            default: break;
            }
            // clang-format on
        }
    };

}    // namespace Udon