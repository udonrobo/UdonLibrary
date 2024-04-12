#pragma once

#include "E220Base.hpp"

namespace Udon
{

    /// @brief E220 送信クラス
    /// @tparam T 送信するメッセージ型
    template <typename T>
    class E220Writer
        : public E220Base
    {
    public:
        /// @brief メッセージサイズ
        static constexpr size_t Size = Udon::SerializedSize<T>();

        /// @brief メッセージ型
        using MessageType = T;

        /// @brief コンストラクタ
        /// @param config 設定
        E220Writer(const Config& config)
            : E220Base(config)
        {
        }

        /// @brief 送信中かどうか
        /// @param timeoutMs タイムアウト時間
        /// @return 送信中ならtrue
        bool isTransmitting(uint32_t timeoutMs = 200) const
        {
            return millis() - lastTransmitMs < timeoutMs;
        }

        /// @brief 送信開始
        void begin()
        {
            E220Base::begin();
        }

        /// @brief メッセージを送信
        void setMessage(const MessageType& message)
        {
            if (digitalRead(config.aux) == HIGH)
            {
                uint8_t buffer[Size];
                Udon::Serialize(message, buffer);
                config.serial.write(uint8_t(config.address >> 0));
                config.serial.write(uint8_t(config.address >> 8));
                config.serial.write(uint8_t(config.channel     ));
                config.serial.write(buffer, sizeof buffer);
                lastTransmitMs = millis();
            }
        }

    private:
        uint32_t lastTransmitMs = 0;
    };

}    // namespace Udon