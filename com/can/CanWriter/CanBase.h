#pragma once
#include <FlexCAN_T4.h>

class CanBase {
	protected:
		using Can = FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256>;

		static Can& canReference() {
			static Can canReference;
			return canReference;
		}
		
		static void begin(bool restart = false) {
			static bool isBegined;
			if (isBegined && not restart) return;
			isBegined = true;

			// 通信が開始されていない場合通信開始
			Can& can = canReference();
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
};
