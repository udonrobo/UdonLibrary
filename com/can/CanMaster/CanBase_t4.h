#pragma once

#include <FlexCAN_T4.h>
#include <IntervalTimer.h>

class CanBase {
	protected:
		static constexpr uint8_t MASTER_ID = 255;  // マスターID(固定)
		static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> can;

		// インスタンス間通信用メンバ
		static IntervalTimer timer;

		/**
		    @brief 受信割り込み(インスタンス間通信用)
		    @param msg CAN_message_t構造体
		*/
		static void onAllReceive(const CAN_message_t& msg) {
			const uint8_t writerID = msg.id;      // 送信元ID
			const uint8_t targetID = msg.buf[0];  // ターゲットID

			//			if (targetID == MASTER_ID)            // 自身(マスター)に向けた送信の場合
			//				for (const auto& p : instancePtr)
			//					if (writerID == p->observID)      // ターゲットIDと観察するIDが一致する場合,そのインスタンスのonReceiveを呼ぶ
			//						p->onReceive(msg);
		}

		static void begin(bool restart = false) {
			{
				static bool isBegined;
				if (isBegined && not restart) return;
				isBegined = true;
			}

			// 通信が開始されていない場合通信開始
			can.begin();
			can.setClock(CLK_60MHz);
			can.setBaudRate(1000000);
			can.enableFIFO();
			can.enableFIFOInterrupt();

//			if (instancePtr.empty()) {  // 最初のインスタンス
//				can.onReceive(onAllReceive);
//				// タイマー割り込みで通信を監視(200us間隔)
//				timer.begin([] {can.events();}, 200);
//			}
		}

		///@brief インスタンスごとの受信割り込み
		template<class T>
		void append(uint8_t id, T (*callback)(CAN_message_t&))
		{
		}
};

// static メンバ初期化
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> CanBase::can;
IntervalTimer CanBase::timer;
