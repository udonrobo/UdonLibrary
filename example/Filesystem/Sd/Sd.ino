//
//    SD カードパラーメーター保存 スケッチ例
//
//    Copyright (c) 2022-2023 Maeda Tomoki
//    Copyright (c) 2022-2024 udonrobo
//
//    teensy のみ対応
//

#include <Udon.hpp>

using Udon::Parameter;

const int   dataSize = 10;
const char* fileName = "parameter.csv";

Parameter myParameter(dataSize, fileName);

void setup()
{
    Serial.begin(115200);
    myParameter.readParameter();
    myParameter.readShow();
    for (int i = 0; i < 10; i++)
        myParameter.setParameter(i + 1, random(100));
    myParameter.writeShow();
    myParameter.writeParameter();
}

void loop()
{
}
