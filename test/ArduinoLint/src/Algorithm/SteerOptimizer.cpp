//
//    構文テスト
// 
//    Copyright (c) 2022 udonrobo
//

#include <Arduino.h>

#include <Udon/Algorithm/SteerOptimizer.hpp>

__attribute__((unused)) 
static void test()
{
    {
        Udon::SteerModuleOptimizer o;
        o.clear();
        (void)o({ 0.0, 0.0 }, { 0.0, 0.0 });
    }

    {
        Udon::SteerOptimizer<4> o;
        o.clear();
        std::array<Udon::Polar, 4> current;
        std::array<Udon::Polar, 4> target;
        (void)o(current, target);
    }
}