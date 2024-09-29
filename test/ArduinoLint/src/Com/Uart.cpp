//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Com/Serial.hpp>

static void test()
{
    Udon::SerialReader<int> reader{ Serial };
    reader.getMessage();
    reader.show();
    reader.showRaw();

    Udon::SerialWriter<int> writer{ Serial };
    writer.setMessage(0);
    writer.show();
    writer.showRaw();
}
