/**
    CAN通信マスター(teensy用)
*/

#include "LoopCycleController.h"
#include "CanMasterReader_t4.h"
#include "CanMasterWriter_t4.h"

LoopCycleController loopCtrl(10000);

const uint8_t slaveID = 0;
const uint8_t dataSize = 30;
CanMasterReader canr(slaveID, dataSize);
CanMasterWriter canw(slaveID, dataSize);

void setup() {
	Serial.begin(115200);
	delay(1000);
}

void loop() {

	{	// 送信部
		for (uint8_t i = 0; i < canw.size(); i++)
			canw[i] = millis();  // 添え字演算子をオーバーロードするとこのように使えて便利です
		canw.update();
	}

	{	// 受信部
		canr.show('\n');
	}

	loopCtrl.Update();
}
