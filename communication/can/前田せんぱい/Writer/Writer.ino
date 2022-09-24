#include"LoopCycleController.h"
#include"CAN_Writer_Teensy4X.h"

const uint8_t MyAddress = 1;
const uint8_t SlaveAddress = 2;
const int SendSize = 6;

LoopCycleController loopCtrl(10000);
CANWriter test(MyAddress, SlaveAddress, SendSize);

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  CAN_begin();
}

byte n = 0;
unsigned long time;

void loop() {
  n++;
  for (int i = 0; i < SendSize; i++) {
    test.SetSingleData(i + 1, n);
  }
  digitalWrite(13, HIGH);
  test.Update();
  digitalWrite(13, LOW);
  test.show();
  Serial.print(micros() - time);
  time = micros();
  Serial.println();
  loopCtrl.Update();
}
