/// @file   CanReader.hpp
/// @date   2022/09/27
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

template<class MessageTy>
class CanReader {
	
		const uint16_t id;
		uint8_t buffer[sizeof(MessageTy)];
		uint32_t lastReceiveMs;
		bool* instanceAlived;
		
	public:

		/// @param id 信号識別ID ~127
		template<class BusTy>
		CanReader(BusTy& bus, const uint16_t id)
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
			MessageTy retval;
			memcpy(&retval, buffer, sizeof retval);
			return retval;
		}
};
