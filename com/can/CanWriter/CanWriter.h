/// @file   CanWriter.h
/// @date   2022/09/27
/// @brief  CASize通信送信クラス
/// @format head:[id+index] data:[8bytes]
/// @author 大河 祐介

#include "CanBase.h"

#pragma once

template<uint8_t Size>
class CanWriter : private CanBase {

		const uint16_t id;
		uint8_t buffer[Size];

		uint8_t index;
	public:

		/// @param id 信号識別ID ~127
		CanWriter(const uint16_t id)
			: id(id)
		{
			CanBase::begin();
		}

		void update() {
			constexpr uint8_t packetSize = ceil(Size / 7.0);
			
			/// パケットに分けて送信
			/// index : コールバック関数がthisポインタ経由で変数にアクセスするためメンバ変数化
			for (index = 0; index < packetSize; index++) {
				
				CanBase::write([](Message_t& msg, void* p) {
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

		uint8_t& operator[](uint8_t index) {
			return buffer[index];
		}
		CanWriter& operator=(const uint8_t& r) {
			memset(buffer, r, Size);
			return *this;
		}
		constexpr uint8_t size() const {
			return Size;
		}

		void show(const char end = {}) const {
			for (const auto& buf : buffer)
				Serial.print(buf), Serial.print('\t');
			Serial.print(end);
		}

};
