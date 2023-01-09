/// @file   CanReader.hpp
/// @date   2022/09/27
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

#include "CanBase.hpp"
#include "FunctionBinder.hpp"

template<class MessageTy>
class CanReader
	: private CanBase
	, private CanBase::FunctionBinder_t
{
		static constexpr size_t Size = sizeof(MessageTy);
		const uint16_t id;
		uint8_t buffer[Size];
		uint32_t lastReceiveMs;
	public:

		/// @param id 信号識別ID ~127
		CanReader(const uint16_t id)
			: id(id)
			, buffer{}
			, lastReceiveMs()
		{
			CanBase::begin();
		}

		bool isOpen() const {
			return millis() - lastReceiveMs < 30;
		}

		MessageTy getMessage() const {
			MessageTy retval;
			memcpy(&retval, buffer, Size);
			return retval;
		}

	private:
		/// @brief 受信割り込み
		void callback(const Message_t& msg) override {
			if (msg.id == id) {
				for (uint8_t i = 0; i < Message_t::dataLength; i++) {
					const uint8_t bufIndex = i + msg.index * Message_t::dataLength;
					if (bufIndex < Size)  // 配列範囲
						buffer[bufIndex] = msg.data[i];
					else
						break;
				}
				lastReceiveMs = millis();
			}
		}
};
