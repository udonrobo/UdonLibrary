#pragma once

#include <Udon/Algorithm/Input.hpp>
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
        E220Reader(const Config& config)
            : E220Base(config)
        {
        }

        /// @brief 受信中かどうか
        /// @param timeoutMs タイムアウト時間
        /// @return 受信中ならtrue
        bool isReceiving(uint32_t timeoutMs = 200) const
        {
            return millis() - lastReceiveMs < timeoutMs;
        }

        /// @brief 受信開始
        void begin()
        {
            E220Base::begin();
        }

        /// @brief メッセージを受信
        /// @return メッセージ (受信エラー時は nullopt)
        Udon::Optional<MessageType> getMessage()
        {
            auxInput.update(digitalRead(config.aux));

            if (auxInput.click)    // 全データがLoRaから送信された場合、aux が立ち上がる
            {
                if (Size == config.serial.available())
                {
                    uint8_t buffer[Size];
                    config.serial.readBytes(buffer, sizeof buffer);

                    const auto message = Udon::Deserialize<MessageType>(buffer);
                    if (message.hasValue())
                    {
                        lastReceiveMs = millis();
                    }
                    return message;
                }
                else
                {
                    while (config.serial.available())
                        (void)config.serial.read();
                }
            }

            return Udon::nullopt;
        }

    private:
        Udon::Input auxInput;

        uint32_t lastReceiveMs = 0;
    };

}    // namespace Udon