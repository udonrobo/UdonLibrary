/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

#include "functional.hpp"

template<class MessageTy>
class CanReader {

		const uint32_t id;
		uint8_t buffer[sizeof(MessageTy)];
		uint32_t timestamp;

		udon::std::function<void()> detach;

		struct Handler {
			uint32_t id;
			uint8_t* buffer;
			size_t size;
			uint32_t* timestamp;
			void* _this;
		};
		
	public:

		/// @param id 信号識別ID
		template<class Bus>
		CanReader(Bus& bus, const uint32_t id)
			: id(id)
			, buffer()
			, timestamp()
			, detach([&] { bus.detach(*this); })
		{
			bus.join(*this);
		}

		~CanReader()
		{
			detach();
		}

		/// @brief 通信できているか
		operator bool() const noexcept
		{
			return micros() - timestamp < 50000;
		}

		/// @brief メッセージ構造体を取得
		MessageTy getMessage() const noexcept
		{
			MessageTy msg;
			memcpy(&msg, buffer, sizeof msg);
			return msg;
		}

		/// @brief 受信内容を表示
		void show(char end = {}) const noexcept
		{
			getMessage().show();
			Serial.print(end);
		}

		/// @brief Readerインスタンスのハンドラ取得(Bus用)
		Handler getHandler()
		{
			return {
				id,
				buffer,
				sizeof buffer,
				&timestamp,
				this
			};
		}
		
};
