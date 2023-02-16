/// @file   CANReader.hpp
/// @date   2023/01/15
/// @brief  CAN通信受信クラス
/// @author 大河 祐介

#pragma once

#include "CANInfo.hpp"
#include "CANBusInterface.hpp"

template<class MessageTy>
class CANReader {

		CANBusInterface& bus                      ;
		CANNodeInfo      node                     ;
		uint8_t          buffer[sizeof(MessageTy)];

	public:

		/// @param id 信号識別ID
		CANReader(CANBusInterface& bus, const uint32_t id)
			: bus   { bus                           }
			, node  { id, buffer, sizeof(MessageTy) }
			, buffer{                               }
		{
			bus.joinRX(node);
		}

		/// @param コピーコンストラクタ
		CANReader(const CANReader& rhs)
			: bus   { rhs.bus                                }
			, node  { rhs.node.id, buffer, sizeof(MessageTy) }
			, buffer{                                        }
		{
			bus.joinRX(node);
		}

		~CANReader()
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
