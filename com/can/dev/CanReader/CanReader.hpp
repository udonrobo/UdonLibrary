#pragma once

#include "CanBus.hpp"
#include "BasicReader.hpp"

template<size_t Size, int Bus>
class CanReader : public BasicReader<Size> {

		using CanBus_t = CanBus<Bus>;

		CanBus_t& bus;
		uint16_t id;
		uint8_t buffer[Size];

	public:
	
		/// @brief Readerインスタンスを生成し通信を開始
		/// @param bus CanBusクラスインスタンス
		/// @param id  読み取るID
		CanReader(CanBus_t& bus, const uint16_t id)
			: bus(bus)
			, id(id)
			, buffer()
		{
			bus.onReceive(id);// CanBus_t::BeginSatus::EnableReadIntrrupt reinterpret_cast<void*>(this)
		}

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
