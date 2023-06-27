//-----------------------------------------------
//
//	  UdonLibrary
//
//	  Copyright (c) 2022-2023 Maeda Tomoki
//	  Copyright (c) 2022-2023 udonrobo
//
//	  Licensed under the MIT License.
//
//-----------------------------------------------
//
//    SD カードパラーメーター保存 スケッチ例
//
//-----------------------------------------------

// teensy のみ対応

#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)

#include <Udon.hpp>

using udon::Parameter;


const int dataSize = 10;
const char* fileName = "parameter.csv";

Parameter myParameter(dataSize, fileName);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myParameter.readParameter();
  myParameter.readShow();
  for (int i = 0; i < 10; i++)
    myParameter.setParameter(i + 1, random(100));
  myParameter.writeShow();
  myParameter.writeParameter();
}

void loop() {
  // put your main code here, to run repeatedly:

}

#else


void setup() {}
void loop() {}

#endif