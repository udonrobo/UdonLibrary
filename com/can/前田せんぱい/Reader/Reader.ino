#include"CAN_Reader_Teensy4X.h"
#include"LoopCycleController.h"

const uint8_t MyAddress = 2;
const uint8_t MasterAddress = 1;
const int ReadSize = 6;

CANReader test(MyAddress);
LoopCycleController loopCtrl(1000);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  CAN_begin();
  Can1.onReceive(receiveEvent);//受け取りの設定
  test.SetAddress(MasterAddress, ReadSize);
  while (millis() < 1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  test.Update();
  test.show(MasterAddress);
  //  Serial.print(test.GetTime(MasterAddress)); Serial.print("\t");
  Serial.println();
  digitalWrite(13, LOW);
  loopCtrl.Update();
}

void receiveEvent(CAN_message_t &msg) {

  //  do {
  test.read(msg);
  Serial.println("call");
  //  } while (test.reading());
}
