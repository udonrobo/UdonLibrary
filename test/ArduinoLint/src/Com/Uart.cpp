//
//    構文テスト
//
//    Copyright (c) 2022 udonrobo
//

#include <Arduino.h>
#include <Udon/Com/Serial.hpp>

__attribute__((unused)) static void test()
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
