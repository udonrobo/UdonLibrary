/// @file   CanReader.hpp
/// @date   2022/09/27
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

template<class MessageTy>
class CanReader {
	
		const uint16_t id;
		uint8_t buffer[sizeof(MessageTy)];
		bool* instanceAlived;
		
	public:

		/// @param id 信号識別ID ~127
		template<class BusTy>
		CanReader(BusTy& bus, const uint16_t id)
			: id(id)
			, buffer{}
		{
			instanceAlived = bus.joinReader(id, buffer);
		}

		~CanReader() {
			*instanceAlived = false;
		}

		MessageTy getMessage() const {
			MessageTy retval;
			memcpy(&retval, buffer, sizeof retval);
			return retval;
		}
};
