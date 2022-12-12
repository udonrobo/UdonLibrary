#pragma once

#include "CanBus.hpp"
#include "BasicWriter.hpp"

template<size_t Size>
class CanWriter : public BasicWriter<Size> {

		CanBus& bus;
		uint16_t id;
		uint8_t buffer[Size];

	public:

		/// @brief インスタンス生成
		/// @param bus CanBusインスタンス参照
		/// @param id  自身のID
		CanWriter(CanBus& bus, const uint16_t id)
			: BasicWriter<Size>(buffer)
			, bus(bus)
			, id(id)
			, buffer()
		{}

		void update() {
			constexpr uint8_t packetSize = ceil(Size / CanBus::DATA_LENGTH_AT_PACKET);

			/// パケットに分けて送信
			for (uint8_t pid = 0; pid < packetSize; pid++) {

				bus.beginPacket(pid);

				for (uint8_t i = 0; i < CanBus::DATA_LENGTH_AT_PACKET; i++) {
					const uint8_t bufferIndex = i + pid * CanBus::DATA_LENGTH_AT_PACKET;
					if (bufferIndex < Size)
						bus.write(i, buffer[bufferIndex]);
					else
						break;
				}

				bus.endPacket();

			}
		}

};
