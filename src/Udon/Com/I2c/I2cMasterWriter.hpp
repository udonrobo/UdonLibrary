//
//    I2c マスター側送信クラス
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//

//
//    master --[I2C]--> slave
//    ^^^^^^
//

#pragma once

#include "I2cBus.hpp"

#include <Udon/Serializer/Serializer.hpp>
#include <Udon/Common/Show.hpp>
#include <Udon/Com/Common/ParsableArray.hpp>
#include <Udon/Com/Common/ArrayElementWriter.hpp>

namespace Udon
{

    template <typename Message>
    class I2cMasterWriter
    {
    public:
        /// @brief 受信メッセージ型
        using MessageType = Message;

        /// @brief 受信バッファサイズ
        static constexpr size_t Size = Udon::SerializedSize<MessageType>();

        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param address スレーブアドレス
        I2cMasterWriter(Udon::II2cBus& bus, uint8_t address)
            : bus(bus)
            , address(address)
            , buffer()
        {
        }

        /// @brief 送信するメッセージを設定、送信
        /// @param message 送信するメッセージ
        void setMessage(const MessageType& message)
        {
            Udon::Serialize(message, buffer);
            bus.beginTransmission(address);
            bus.write(buffer, Size);
            bus.endTransmission();
        }

        /// @brief 送信内容を表示
        void show() const
        {
            if (const auto message = Udon::Deserialize<MessageType>(buffer))
            {
                Udon::Show(*message);
            }
            else
            {
                Udon::Show(F("unpack failed!"));    // 通常はここには到達しない
            }
        }

        /// @brief 送信バッファを表示
        void showRaw(char gap = ' ') const
        {
            Udon::Show(buffer);
        }

    private:
        Udon::II2cBus& bus;

        uint8_t address;

        uint8_t buffer[Size];
    };

    /// @brief メッセージ配列受信クラス
    /// @details
    ///     メッセージ配列を受信するためのクラスです。
    ///     I2cMasterWriter<Udon::Vec2[5]> reader(bus, address); のように使用します。
    ///     at メソッドで各要素を Reader として取得できます。
    ///     Udon::Encoder<Udon::ArrayElementReader> encoder(reader.at(index)); のように使用します。
    ///     通常の I2cMasterWriter を継承しているため、I2cMasterWriter 内のメソッドをそのまま使用できます。
    /// @tparam Message メッセージ型
    /// @tparam N 配列要素数
    template <typename Message, size_t N>
    class I2cMasterWriter<Message[N]>
        : public I2cMasterWriter<Udon::ParsableArray<Message, N>>
    {

        using Writer = I2cMasterWriter<Udon::ParsableArray<Message, N>>;

        using ArrayType = typename Writer::MessageType;

        ArrayType array;

    public:
        /// @brief コンストラクタ
        using Writer::Writer;

        /// @brief 配列要素を Reader として取得
        Udon::ArrayElementWriter<Message> at(size_t index)
        {
            return { array.at(index) };
        }

        /// @brief 更新
        void update()
        {
            Writer::setMessage(array);
            Writer::update();
        }
    };

}    // namespace Udon
