//
//    構文テスト
//
//    Copyright (c) 2022 udonrobo
//

#include <Udon/Filesystem/Sd/Parameter.hpp>

#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)

__attribute__((unused)) static void test()
{
    Udon::Parameter p(10, "hoge.csv");

    p.readParameter();
    p.readShow();
    for (int i = 0; i < 10; i++)
        p.setParameter(i + 1, random(100));
    p.writeShow();
    p.writeParameter();
}

#endif