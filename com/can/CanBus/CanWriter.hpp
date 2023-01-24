/// @file   CanWriter.hpp
/// @date   2023/01/15
/// @brief  CAN通信送信クラス
/// @author 大河 祐介

#pragma once

#include "functional.hpp"

template<class MessageTy>
class CanWriter {

		const uint32_t id;
		uint8_t buffer[sizeof(MessageTy)];
		uint32_t timestamp;

		udon::std::function<void(CanWriter&)> join;
		udon::std::function<void(CanWriter&)> detach;

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
		CanWriter(Bus& bus, const uint32_t id)
			: id(id)
			, buffer()
			, timestamp()
			, join  ([&](CanWriter& self) { bus.join  (self); })
			, detach([&](CanWriter& self) { bus.detach(self); })
		{
			join(*this);
		}

		~CanWriter()
		{
			detach(*this);
		}

		/// @brief 通信できているか
		operator bool() const noexcept
		{
			return micros() - timestamp < 50000;
		}

		/// @brief メッセージ構造体をセット
		void setMessage(const MessageTy& message) noexcept
		{
			memcpy(buffer, &message, sizeof buffer);
		}

		/// @brief 受信内容を表示
		void show(char end = {}) const noexcept
		{
			MessageTy msg;
			memcpy(&msg, buffer, sizeof msg);
			msg.show();
			Serial.print(end);
		}

		/// @brief Writerインスタンスのハンドラ取得(Bus用)
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
