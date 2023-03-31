///   @file   I2cSlaveReader.hpp
///   @brief  I2cスレーブ送信用クラス
///   @author 大河 祐介
///   @date   2023/03/31

#pragma once

#include <udon\com\i2c\I2cBus.hpp>

#include <udon\com\serializer\serializer.hpp>

namespace udon
{


    template<class Message>
    class I2CSlaveWriter
    {

	public:

		static constexpr size_t Size = Message::PackedSize;

	private:

		udon::I2cBus& bus;

		uint8_t buffer[Size];

		static I2CSlaveWriter* self;

    public:

        /// @param address I2cアドレス
        /// @param clock   通信レート
        I2CSlaveWriter(udon::I2cBus& bus)
			: bus(bus)
			, buffer()
        {
			self = this;
            bus.onRequest([] {
				self->bus.write(self->buffer, Size);
			});
        }

		void setMessage(const Message& message)
		{
			const auto packed = udon::Pack(message);
			std::copy(buffer, packed.begin(), packed.end());
		}

        /// @brief 送信内容を表示
        /// @param end   オプション [\n, \t ..]
        /// @param radix 基数      [BIN, HEX ..]
        void show(const char end = {}, const uint16_t radix = DEC) const noexcept
        {
            for (const auto& buffer : buffer)
                Serial.print(buffer, radix), Serial.print('\t');
            Serial.print(end);
        }

    };

    template<class Message>
    I2CSlaveWriter<Message>* I2CSlaveWriter<Message>::self;

}    // namespace udon
