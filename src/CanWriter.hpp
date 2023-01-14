/// @file   CanWriter.hpp
/// @date   2022/09/27
/// @brief  CAN通信送信クラス
/// @author 大河 祐介

#pragma once

template<class MessageTy>
class CanWriter {

		const uint16_t id;
		uint8_t buffer[sizeof(MessageTy)];
		uint32_t lastSendMs;
		bool* instanceAlived;

	public:

		/// @param id 信号識別ID
		template<class BusTy>
		CanWriter(BusTy& bus, const uint16_t id)
			: id(id)
			, buffer()
			, lastSendMs()
			, instanceAlived(bus.joinWriter(id, buffer, lastSendMs))
		{}

		~CanWriter() noexcept
		{
			*instanceAlived = false;
		}

		operator bool() const noexcept {
			return millis() - lastSendMs < 50;
		}

		void setMessage(const MessageTy& message) noexcept
		{
			memcpy(buffer, &message, sizeof buffer);
		}

};
