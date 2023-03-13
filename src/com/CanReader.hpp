/// @file   CanReader.hpp
/// @date   2023/01/15
/// @brief  Can通信受信クラス
/// @author 大河 祐介

#pragma once

#include "CanInfo.hpp"
#include "CanBusInterface.hpp"

template<class MessageTy>
class CanReader {

		CanBusInterface& bus                      ;
		CanNodeInfo      node                     ;
		uint8_t          buffer[sizeof(MessageTy)];

	public:

		/// @param id 信号識別ID
		CanReader(CanBusInterface& bus, const uint32_t id)
			: bus   { bus                           }
			, node  { id, buffer, sizeof(MessageTy) }
			, buffer{                               }
		{
			bus.joinRX(node);
		}

		/// @param コピーコンストラクタ
		CanReader(const CanReader& rhs)
			: bus   { rhs.bus                                }
			, node  { rhs.node.id, buffer, sizeof(MessageTy) }
			, buffer{                                        }
		{
			bus.joinRX(node);
		}

		~CanReader()
		{
			bus.detachRX(node);
		}

		constexpr size_t length() const noexcept
		{
			return sizeof(MessageTy);
		}

		/// @brief 通信できているか
		operator bool() const noexcept
		{
			return micros() - node.timestampUs < 50000;
		}

		/// @brief メッセージ構造体を取得
		MessageTy getMessage() const noexcept
		{
			MessageTy msg;
			memcpy(&msg, node.buffer, sizeof msg);
			return msg;
		}

		/// @brief 受信内容を表示
		void show(char end = {}) const noexcept
		{
			getMessage().show();
			Serial.print(end);
		}

};
