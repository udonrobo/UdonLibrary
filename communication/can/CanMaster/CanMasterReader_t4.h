#pragma once

#include "CanBase_t4.h"

#include <FlexCAN_T4.h>
#include <IntervalTimer.h>
#include <vector>

class CanMasterReader : private CanBase {

		// インスタンス間通信用メンバ
		static IntervalTimer timer;
		static std::vector<CanMasterReader*> instancePtr;  // インスタンスごとのthisポインタ用配列

		// 通常メンバ
		const uint8_t observID;  // 観察する送信元ID
		const uint8_t readSize;  // 読み取りサイズ
		uint8_t *buf;            // 読み取りバッファ

	public:
		CanMasterReader(uint8_t observID, uint8_t readSize)
			: observID(observID)
			, readSize(readSize)
			, buf(new uint8_t[readSize])
		{
			memset(buf, 0, readSize);
			CanBase::begin();
			if (instancePtr.empty()) {  // 最初のインスタンス
				can.onReceive(onAllReceive);
				// タイマー割り込みで通信を監視(200us間隔)
				timer.begin([] {can.events();}, 200);
			}
			instancePtr.push_back(this);  // インスタンスのアドレスを追加
		}
		~CanMasterReader() {
			delete[] buf;
		}

		const uint8_t& operator[](uint8_t index) const {
			return buf[index];
		}
		uint8_t size() const {
			return readSize;
		}

		void show(const String& str = "") const {
			for (uint8_t i = 0; i < readSize; i++)
				Serial.print(buf[i]), Serial.print('\t');
			Serial.print(str);
		}

	private:
		/**
		    @brief 受信割り込み
		    @param msg CAN_message_t構造体
		*/
		void onReceive(const CAN_message_t& msg) {

			// communication format [8byte]
			// [target address][packet index][data][data][data][data][data][data]

			const uint8_t packetIndex = msg.buf[1];  // 分割したデータの要素番号

			for (uint8_t i = 0; i < 6; i++) {
				const uint8_t bufferIndex = i + packetIndex * 6;
				if (bufferIndex < readSize)  // 配列範囲内参照時
					buf[bufferIndex] = msg.buf[i + 2/*先頭2byteを飛ばす*/];
				else
					break;
			}
		}

		/**
		    @brief 受信割り込み(インスタンス間通信用)
		    @param msg CAN_message_t構造体
		*/
		static void onAllReceive(const CAN_message_t& msg) {
			const uint8_t writerID = msg.id;      // 送信元ID
			const uint8_t targetID = msg.buf[0];  // ターゲットID

			if (targetID == MASTER_ID)            // 自身(マスター)に向けた送信の場合
				for (const auto& p : instancePtr)
					if (writerID == p->observID)      // ターゲットIDと観察するIDが一致する場合,そのインスタンスのonReceiveを呼ぶ
						p->onReceive(msg);

		}

};

// static メンバ初期化
IntervalTimer CanMasterReader::timer;
std::vector<CanMasterReader*> CanMasterReader::instancePtr;
