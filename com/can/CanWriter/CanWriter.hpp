/// @file   CanWriter.hpp
/// @date   2022/09/27
/// @brief  CAN通信送信クラス
/// @author 大河 祐介

#pragma once

#include "CanBase.hpp"
#include "BasicWriter.hpp"


template<uint8_t Size>
class CanWriter
	: private CanBase
	, public BasicWriter<Size>
{
		const uint16_t id;
		uint8_t index;
		uint8_t buffer[Size];
	public:

		/// @param id 信号識別ID ~127
		CanWriter(const uint16_t id) noexcept
			: BasicWriter<Size>(buffer)
			, id(id)
			, index()
			, buffer{}
		{
			CanBase::begin();
		}

		/// @brief 送信
		void update() {
			constexpr uint8_t packetSize = ceil(Size / 7.0);  /// 1パケットにデータ7byte

			/// パケットに分けて送信
			for (index = 0; index < packetSize; index++) {

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
