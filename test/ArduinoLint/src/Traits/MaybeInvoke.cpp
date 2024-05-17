//
//    構文テスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <Udon/Traits/HasMemberFunction.hpp>

struct TestStructure1
{
    void begin() {}
};
struct TestStructure2
{
};
inline void test()
{
    TestStructure1 a;
    Udon::Traits::MaybeInvokeBegin(a);    // call

    TestStructure2 b;
    Udon::Traits::MaybeInvokeBegin(b);    // not call
}