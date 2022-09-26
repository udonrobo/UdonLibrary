#include "CanMasterWriter.h"
#include "CanSlaveWriter.h"
#include "LoopCycleController.h"

LoopCycleController loopCtrl(1000);

const uint8_t myID = 0;
CanSlaveWriter can(myID, 24);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop() {

  //  const uint32_t t = micros();

  for (uint8_t i = 0; i < can.size(); i++)
    can[i] = i;

  can.show('\n');

  can.update();

  //  Serial.println(micros() - t);  // 3us でした

  loopCtrl.Update();
}
