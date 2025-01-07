//
//    Show
//
//    Copyright (c) 2022 udonrobo
//

#include <Arduino.h>
#include <Udon/Utility/Show.hpp>
#include <Udon/Traits/EnumerableMacro.hpp>

struct Enumerable
{
    int value;
    UDON_ENUMERABLE(value);
};

__attribute__((unused)) static void test()
{
    // 組み込み型
    {
        Udon::Show(100);
        Udon::Show("Hello, World!");
        Udon::Show(100, 100.0, "100");
    }

    // 構造体
    {
        Enumerable e{ 100 };
        Udon::Show(e);
    }

    // 配列
    {
        // int array[] = { 1, 2, 3, 4, 5 };
        // Udon::Show(array);
    }
}
