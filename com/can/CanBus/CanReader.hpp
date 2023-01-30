/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

#include "CanHandle.hpp"
#include "CanBusInterface.hpp"

template<class MessageTy>
class CanReader {

		CanBusInterface& hBus                     ;
		CanNodeHandle    hNode                    ;
		uint8_t          buffer[sizeof(MessageTy)];

	public:

		/// @param id 信号識別ID
		CanReader(CanBusInterface& hBus, const uint32_t id)
			: hBus  { hBus                          }
			, hNode { id, buffer, sizeof(MessageTy) }
			, buffer{                               }
		{
			hBus.joinRX(hNode);
		}

		~CanReader()
		{
			hBus.detachRX(hNode);
		}

		constexpr size_t length() const noexcept
		{
			return sizeof(MessageTy);
		}

		/// @brief 通信できているか
		operator bool() const noexcept
		{
			return micros() - hNode.timestampUs < 50000;
		}

		/// @brief メッセージ構造体を取得
		MessageTy getMessage() const noexcept
		{
			MessageTy msg;
			memcpy(&msg, hNode.buffer, sizeof msg);
			return msg;
		}

		/// @brief 受信内容を表示
		void show(char end = {}) const noexcept
		{
			getMessage().show();
			Serial.print(end);
		}

};
