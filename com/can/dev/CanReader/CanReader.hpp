#pragma once

#include "CanBus.hpp"
#include "BasicReader.hpp"

template<size_t Size, CAN_DEV_TABLE Bus>
class CanReader : public BasicReader<Size> {

		CanBus<Bus>& bus;
		uint16_t id;
		uint8_t buffer[Size];

	public:
	
		/// @brief Readerインスタンスを生成し通信を開始
		/// @param bus CanBusクラスインスタンス
		/// @param id  読み取るID
		CanReader(CanBus<Bus>& bus, const uint16_t id)
			: BasicReader<Size>(buffer)
			, bus(bus)
			, id(id)
			, buffer()
		{
			bus.appendMailBox(id, buffer);
		}

		~CanReader() = delete;

	private:

//		/// @brief 受信割り込み
//		void callback(const Message_t& msg) override {
//			for (uint8_t i = 0; i < Message_t::dataLength; i++) {
//				const uint8_t bufIndex = i + msg.index * Message_t::dataLength;
//				if (bufIndex < Size)  // 配列範囲
//					buffer[bufIndex] = msg.data[i];
//				else
//					break;
//			}
//		}
};
