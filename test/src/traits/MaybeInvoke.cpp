#include <Udon/Traits/MaybeInvoke.hpp>

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
    Udon::MaybeInvoke_begin(a);    // call

    TestStructure2 b;
    Udon::MaybeInvoke_begin(b);    // not call
}