/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

#include "memory.hpp"

template<class MessageTy>
class CanReader {

		const uint32_t id;
		uint8_t buffer[sizeof(MessageTy)];
		uint32_t lastReceiveMs;
		udon::std::shared_ptr<bool> instanceAlived;

	public:

		/// @param id 信号識別番号
		template<class BusTy>
		CanReader(BusTy& bus, const uint32_t id)
			: id(id)
			, buffer()
			, lastReceiveMs()
			, instanceAlived(bus.joinReader(id, buffer, lastReceiveMs))
		{}

		~CanReader() noexcept {
			*instanceAlived = false;
		}

		operator bool() const noexcept {
			return millis() - lastReceiveMs < 50;
		}

		MessageTy getMessage() const noexcept {
			MessageTy msg;
			memcpy(&msg, buffer, sizeof msg);
			return msg;
		}

		void show(char end = {}) const noexcept {
			getMessage().show();
			Serial.print(end);
		}
};
