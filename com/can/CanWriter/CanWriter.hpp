/// @file   CanWriter.hpp
/// @date   2022/09/27
/// @brief  CAN通信送信クラス
/// @author 大河 祐介

#pragma once

#include "CanBase.hpp"
#include "CanBus.hpp"

#include <memory>

template<class MsgTy>
class CanWriter {

		const uint16_t id;
		uint8_t buffer[sizeof(MsgTy)];
		bool* instanceAlived;

	public:

		/// @param id 信号識別ID
		template<class BusTy>
		CanWriter(BusTy& bus, const uint16_t id)
			: id(id)
			, buffer()
			, instanceAlived(new bool(true))
		{
			bus.joinWriter(id, buffer, instanceAlived);
		}

		~CanWriter() {
			*instanceAlived = false;
		}

		void setMessage(const MsgTy& message) {
			memcpy(buffer, &message, sizeof buffer);
		}

		/// @brief 送信
		void update() {
		}

};
