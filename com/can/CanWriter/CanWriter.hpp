/// @file   CanWriter.hpp
/// @date   2022/09/27
/// @brief  CAN通信送信クラス
/// @author 大河 祐介

#pragma once

template<class MessageTy>
class CanWriter {

		const uint16_t id;
		uint8_t buffer[sizeof(MessageTy)];
		bool* instanceAlived;

	public:

		/// @param id 信号識別ID
		template<class BusTy>
		CanWriter(BusTy& bus, const uint16_t id)
			: id(id)
			, buffer()
		{
			instanceAlived = bus.joinWriter(id, buffer);
		}

		~CanWriter() {
			*instanceAlived = false;
		}

		void setMessage(const MessageTy& message) {
			memcpy(buffer, &message, sizeof buffer);
		}

};
