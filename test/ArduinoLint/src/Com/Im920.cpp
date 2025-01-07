//
//    構文テスト
//
//    Copyright (c) 2022 udonrobo
//

#include <Arduino.h>

#ifdef HAVE_HWSERIAL1

#    include <Udon/Com/Im920.hpp>

__attribute__((unused)) static void testIm920()
{
    Udon::Im920 im920{ Serial };
    im920.operator bool();
    im920.begin(0);
    im920.update();
    im920.show();

    Udon::Im920sL im920sL{ Serial, 0 };
    im920sL.operator bool();
    im920sL.begin(0);
    im920sL.update();
    im920sL.show();
}

__attribute__((unused)) static void testReaderWriter()
{
    Udon::Im920 im920{ Serial };

    Udon::Im920Reader<int> reader{ im920 };
    reader.getMessage();
    reader.show();
    reader.showRaw();

    Udon::Im920Writer<int> writer{ im920 };
    writer.setMessage(0);
    writer.show();
    writer.showRaw();
}

#endif    // HAVE_HWSERIAL1