#include <udon/traits/MaybeInvoke.hpp>

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
    udon::MaybeInvoke_begin(a);    // call

    TestStructure2 b;
    udon::MaybeInvoke_begin(b);    // not call
}