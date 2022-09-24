/**
   CAN通信スレーブ(teensy用)
*/

#include "LoopCycleController.h"
#include "CanSlaveReader_t4.h"
#include "CanSlaveWriter_t4.h"

LoopCycleController loopCtrl(1000);

const uint8_t myID = 0;
const uint8_t dataSize = 30;
CanSlaveReader canr(myID, dataSize);
CanSlaveWriter canw(myID, dataSize);

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {

  {  // 送信部
    for (uint8_t i = 0; i < canw.size(); i++)
      canw[i] = millis();
    canw.update();
  }

  {  // 受信部
    if (canr.isConnected())
      canr.show('\n');  // 受信値表示
    else
      Serial.println("unconnected");
  }


  loopCtrl.Update();
}
