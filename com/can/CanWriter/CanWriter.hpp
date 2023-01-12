/// @file   CanWriter.hpp
/// @date   2022/09/27
/// @brief  CAN通信送信クラス
/// @author 大河 祐介

#pragma once

#include "CanBase.hpp"
#include "CanBus.hpp"

template<class MsgTy>
class CanWriter {

		const uint16_t id;
		uint8_t buffer[sizeof(MsgTy)];

	public:

		/// @param id 信号識別ID
		CanWriter(BusInterface& bus, const uint16_t id)
			: id(id)
			, buffer()
		{
			bus.joinWriter(id, buffer);
		}

		void setMessage(const MsgTy& message) {
			memcpy(buffer, &message, sizeof buffer);
		}

		/// @brief 送信
		void update() {
			constexpr uint8_t packetSize = ceil(sizeof buffer / 7.0);  /// 1パケットにデータ7byte

			/// パケットに分けて送信
			for (size_t index = 0; index < packetSize; index++) {

				CanBase::write([](Message_t&& msg, void* p) {
					CanWriter* _this = static_cast<CanWriter*>(p);
					msg.id    = _this->id;
					msg.index = _this->index;
					for (uint8_t i = 0; i < Message_t::dataLength; i++) {
						const uint8_t bufferIndex = i + _this->index * Message_t::dataLength;
						if (bufferIndex < Size)
							msg.data[i] = _this->buffer[bufferIndex];
						else
							break;
					}
				}, this);

			}
		}

};
