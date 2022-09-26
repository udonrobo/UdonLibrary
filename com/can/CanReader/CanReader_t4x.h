#pragma once

#include "CanBase.h"

template<size_t N>
class CanMasterReader : private CanBase, private Callbacker {

		const uint8_t id;   // 観察する送信元ID
		uint8_t buffer[N];  // 読み取りバッファ
		Can& can = CanBase::canReference();

	public:
	
		/// @brief
		/// @param id 監視するID
		CanMasterReader(const uint8_t id)
			: id(id)
		{
			CanBase::begin();
		}

		constexpr const uint8_t& operator[](uint8_t index) const {
			return buffer[index];
		}
		constexpr uint8_t size() const {
			return N;
		}

		void show(const char end = {}) const {
			for (const auto& buf : buffer)
				Serial.print(buf), Serial.print('\t');
			Serial.print(end);
		}

	private:
		/**
		    @brief 受信割り込み
		    @param msg CAN_message_t構造体
		*/
		void onReceive(const CAN_message_t& msg) {

			// communication format [8byte]
			// [targetID][packetIndex][data][data][data][data][data][data]

			const uint8_t packetIndex = msg.buf[1];  // 分割したデータの要素番号

			for (uint8_t i = 0; i < 6; i++) {
				const uint8_t bufIndex = i + packetIndex * 6;
				if (bufIndex < N)  // 配列範囲
					buffer[bufIndex] = msg.buf[i + 2];  /*先頭2byteを飛ばす*/
				else
					break;
			}
		}


};
