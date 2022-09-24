#include "CanMasterReader.h"
#include "LoopCycleController.h"

CanReader can(0, 24);  // CanReader::CanReader(監視するID, 読み取るサイズ)

LoopCycleController loopCtrl(1000);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop() {
  // 表示
  can.show('\n');
  
  loopCtrl.Update();
}
