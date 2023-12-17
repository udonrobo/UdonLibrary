//
//    I2c スレーブ側受信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

//
//    master --[I2C]--> slave
//                      ^^^^^
//

#pragma once

#include "I2cBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>
#include <Udon/Com/Common/ParsableArray.hpp>
#include <Udon/Com/Common/ArrayElementReader.hpp>

namespace Udon
{

    template <typename Message>
    class I2cSlaveReader
    {
    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<MessageType>();

        /// @brief コンストラクタ
        /// @param bus I2cバス
        I2cSlaveReader(Udon::II2cBus& bus)
            : bus(bus)
            , buffer()
        {
            self = this;
        }

        /// @brief コピーコンストラクタ
        /// @param rhs
        I2cSlaveReader(const I2cSlaveReader& rhs)
            : bus(rhs.bus)
            , buffer()
        {
            self = this;
        }

        /// @brief 受信開始
        void begin()
        {
            bus.onReceive(
                [](int)
                {
                    for (auto& buf : self->buffer)
                    {
                        buf = Wire.read();
                    }
                });
        }

        /// @brief 受信したメッセージを取得
        /// @return 受信したメッセージ
        Udon::Optional<MessageType> getMessage() const
        {
            if (bus)
            {
                return Udon::Deserialize<MessageType>(buffer);
            }
            else
            {
                return Udon::nullopt;
            }
        }

        /// @brief 受信内容を表示
        void show() const
        {
            if (const auto message = getMessage())
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        void showRaw() const
        {
            Udon::Show(buffer);
        }

    private:
        Udon::II2cBus& bus;

        uint8_t buffer[Size];

        static I2cSlaveReader* self;
    };

    template <typename Message>
    I2cSlaveReader<Message>* I2cSlaveReader<Message>::self;

    /// @brief メッセージ配列受信クラス
    /// @details
    ///     メッセージ配列を受信するためのクラスです。
    ///     I2cSlaveReader<Udon::Vec2[5]> reader(bus, address); のように使用します。
    ///     at メソッドで各要素を Reader として取得できます。
    ///     Udon::Encoder<Udon::ArrayElementReader> encoder(reader.at(index)); のように使用します。
    ///     通常の I2cSlaveReader を継承しているため、I2cSlaveReader 内のメソッドをそのまま使用できます。
    /// @tparam Message メッセージ型
    /// @tparam N 配列要素数
    template <typename Message, size_t N>
    class I2cSlaveReader<Message[N]>
        : public I2cSlaveReader<Udon::ParsableArray<Message, N>>
    {

        using BaseType = I2cSlaveReader<Udon::ParsableArray<Message, N>>;

        using ArrayType = typename BaseType::MessageType;

        ArrayType array;

        bool hasValue;

    public:
        using BaseType::BaseType;
        
        Udon::ArrayElementReader<Message> at(size_t index)
        {
            return { array.at(index), hasValue };
        }

        void update()
        {
            if (const auto message = BaseType::getMessage())
            {
                array    = *message;
                hasValue = true;
            }
            else
            {
                hasValue = false;
            }
        }
    };

}    // namespace Udon
