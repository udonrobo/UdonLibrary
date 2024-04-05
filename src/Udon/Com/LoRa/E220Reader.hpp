#pragma once

#include <Udon/Algorithm/Input.hpp>
#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Algorithm/ScopedInterruptLocker.hpp>
#include "E220Base.hpp"

namespace Udon
{

    /// @brief E220 受信クラス
    /// @tparam T 受信するメッセージ型
    template <typename T>
    class E220Reader
        : public E220Base
    {
    public:
        /// @brief メッセージサイズ
        static constexpr size_t Size = Udon::SerializedSize<T>();

        /// @brief メッセージ型
        using MessageType = T;

        /// @brief コンストラクタ
        /// @param config 設定
        E220Reader(const E220Base::Config& config)
            : E220Base(config)
        {
            LinkPinToInstance(config.aux, this);
        }

        /// @brief コピーコンストラクタ
        /// @param other 
        E220Reader(const E220Reader& other)
            : E220Base(other.config)
        {
            LinkPinToInstance(config.aux, this);
        }

        /// @brief 受信開始
        void begin() noexcept
        {
            E220Base::begin();
            darkAttachInterrupt(config.aux);
        }

        /// @brief メッセージを受信
        /// @return メッセージ
        /// @note 受信エラー時、受信タイムアウト時は nullopt を返す
        /// @note メッセージは最後に受信したものを返す (受信エラー時は前回のメッセージを返す)
        Udon::Optional<MessageType> getMessage() noexcept
        {
            // 割り込み関数内で使用する変数にアクセスするため排他制御
            ScopedInterruptLocker locker;

            if (received)
            {
                lastReceiveMs = millis();
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

        void OnRisingEdge()
        {
            if (Size == config.serial.available())
            {
                config.serial.readBytes(buffer, sizeof buffer);
                received = true;
            }
        }

        static E220Reader** InstanceList()
        {
            static E220Reader* instanceList[60];
            return instanceList;
        }

        static void LinkPinToInstance(uint8_t pin, E220Reader* self)
        {
            InstanceList()[pin] = self;
        } 

        void darkAttachInterrupt(uint8_t pin)
        {
            // attachInterrupt は this ポインタの情報がないためメンバ関数を呼び出すことができない
            // そこでピン番号とthisポインタを紐づけて、無理やりメンバ関数を呼び出す

            LinkPinToInstance(pin, this);

            // clang-format off
            switch (pin)
            {
            case  0: attachInterrupt( 0, [](){ InstanceList()[ 0]->OnRisingEdge(); }, RISING); break;
            case  1: attachInterrupt( 1, [](){ InstanceList()[ 1]->OnRisingEdge(); }, RISING); break;
            case  2: attachInterrupt( 2, [](){ InstanceList()[ 2]->OnRisingEdge(); }, RISING); break;
            case  3: attachInterrupt( 3, [](){ InstanceList()[ 3]->OnRisingEdge(); }, RISING); break;
            case  4: attachInterrupt( 4, [](){ InstanceList()[ 4]->OnRisingEdge(); }, RISING); break;
            case  5: attachInterrupt( 5, [](){ InstanceList()[ 5]->OnRisingEdge(); }, RISING); break;
            case  6: attachInterrupt( 6, [](){ InstanceList()[ 6]->OnRisingEdge(); }, RISING); break;
            case  7: attachInterrupt( 7, [](){ InstanceList()[ 7]->OnRisingEdge(); }, RISING); break;
            case  8: attachInterrupt( 8, [](){ InstanceList()[ 8]->OnRisingEdge(); }, RISING); break;
            case  9: attachInterrupt( 9, [](){ InstanceList()[ 9]->OnRisingEdge(); }, RISING); break;
            case 10: attachInterrupt(10, [](){ InstanceList()[10]->OnRisingEdge(); }, RISING); break;
            case 11: attachInterrupt(11, [](){ InstanceList()[11]->OnRisingEdge(); }, RISING); break;
            case 12: attachInterrupt(12, [](){ InstanceList()[12]->OnRisingEdge(); }, RISING); break;
            case 13: attachInterrupt(13, [](){ InstanceList()[13]->OnRisingEdge(); }, RISING); break;
            case 14: attachInterrupt(14, [](){ InstanceList()[14]->OnRisingEdge(); }, RISING); break;
            case 15: attachInterrupt(15, [](){ InstanceList()[15]->OnRisingEdge(); }, RISING); break;
            case 16: attachInterrupt(16, [](){ InstanceList()[16]->OnRisingEdge(); }, RISING); break;
            case 17: attachInterrupt(17, [](){ InstanceList()[17]->OnRisingEdge(); }, RISING); break;
            case 18: attachInterrupt(18, [](){ InstanceList()[18]->OnRisingEdge(); }, RISING); break;
            case 19: attachInterrupt(19, [](){ InstanceList()[19]->OnRisingEdge(); }, RISING); break;
            case 20: attachInterrupt(20, [](){ InstanceList()[20]->OnRisingEdge(); }, RISING); break;
            case 21: attachInterrupt(21, [](){ InstanceList()[21]->OnRisingEdge(); }, RISING); break;
            case 22: attachInterrupt(22, [](){ InstanceList()[22]->OnRisingEdge(); }, RISING); break;
            case 23: attachInterrupt(23, [](){ InstanceList()[23]->OnRisingEdge(); }, RISING); break;
            case 24: attachInterrupt(24, [](){ InstanceList()[24]->OnRisingEdge(); }, RISING); break;
            case 25: attachInterrupt(25, [](){ InstanceList()[25]->OnRisingEdge(); }, RISING); break;
            case 26: attachInterrupt(26, [](){ InstanceList()[26]->OnRisingEdge(); }, RISING); break;
            case 27: attachInterrupt(27, [](){ InstanceList()[27]->OnRisingEdge(); }, RISING); break;
            case 28: attachInterrupt(28, [](){ InstanceList()[28]->OnRisingEdge(); }, RISING); break;
            case 29: attachInterrupt(29, [](){ InstanceList()[29]->OnRisingEdge(); }, RISING); break;
            case 30: attachInterrupt(30, [](){ InstanceList()[30]->OnRisingEdge(); }, RISING); break;
            case 31: attachInterrupt(31, [](){ InstanceList()[31]->OnRisingEdge(); }, RISING); break;
            case 32: attachInterrupt(32, [](){ InstanceList()[32]->OnRisingEdge(); }, RISING); break;
            case 33: attachInterrupt(33, [](){ InstanceList()[33]->OnRisingEdge(); }, RISING); break;
            case 34: attachInterrupt(34, [](){ InstanceList()[34]->OnRisingEdge(); }, RISING); break;
            case 35: attachInterrupt(35, [](){ InstanceList()[35]->OnRisingEdge(); }, RISING); break;
            case 36: attachInterrupt(36, [](){ InstanceList()[36]->OnRisingEdge(); }, RISING); break;
            case 37: attachInterrupt(37, [](){ InstanceList()[37]->OnRisingEdge(); }, RISING); break;
            case 38: attachInterrupt(38, [](){ InstanceList()[38]->OnRisingEdge(); }, RISING); break;
            case 39: attachInterrupt(39, [](){ InstanceList()[39]->OnRisingEdge(); }, RISING); break;
            case 40: attachInterrupt(40, [](){ InstanceList()[40]->OnRisingEdge(); }, RISING); break;
            case 41: attachInterrupt(41, [](){ InstanceList()[41]->OnRisingEdge(); }, RISING); break;
            case 42: attachInterrupt(42, [](){ InstanceList()[42]->OnRisingEdge(); }, RISING); break;
            case 43: attachInterrupt(43, [](){ InstanceList()[43]->OnRisingEdge(); }, RISING); break;
            case 44: attachInterrupt(44, [](){ InstanceList()[44]->OnRisingEdge(); }, RISING); break;
            case 45: attachInterrupt(45, [](){ InstanceList()[45]->OnRisingEdge(); }, RISING); break;
            case 46: attachInterrupt(46, [](){ InstanceList()[46]->OnRisingEdge(); }, RISING); break;
            case 47: attachInterrupt(47, [](){ InstanceList()[47]->OnRisingEdge(); }, RISING); break;
            case 48: attachInterrupt(48, [](){ InstanceList()[48]->OnRisingEdge(); }, RISING); break;
            case 49: attachInterrupt(49, [](){ InstanceList()[49]->OnRisingEdge(); }, RISING); break;
            case 50: attachInterrupt(50, [](){ InstanceList()[50]->OnRisingEdge(); }, RISING); break;
            case 51: attachInterrupt(51, [](){ InstanceList()[51]->OnRisingEdge(); }, RISING); break;
            case 52: attachInterrupt(52, [](){ InstanceList()[52]->OnRisingEdge(); }, RISING); break;
            case 53: attachInterrupt(53, [](){ InstanceList()[53]->OnRisingEdge(); }, RISING); break;
            case 54: attachInterrupt(54, [](){ InstanceList()[54]->OnRisingEdge(); }, RISING); break;
            case 55: attachInterrupt(55, [](){ InstanceList()[55]->OnRisingEdge(); }, RISING); break;
            case 56: attachInterrupt(56, [](){ InstanceList()[56]->OnRisingEdge(); }, RISING); break;
            case 57: attachInterrupt(57, [](){ InstanceList()[57]->OnRisingEdge(); }, RISING); break;
            case 58: attachInterrupt(58, [](){ InstanceList()[58]->OnRisingEdge(); }, RISING); break;
            case 59: attachInterrupt(59, [](){ InstanceList()[59]->OnRisingEdge(); }, RISING); break;
            default: break;
            }
            // clang-format on
        }
    };

}    // namespace Udon