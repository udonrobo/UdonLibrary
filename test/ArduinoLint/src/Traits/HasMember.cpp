#include <Udon/Traits/HasMemberFunctionBegin.hpp>
#include <Udon/Traits/HasMemberFunctionShow.hpp>
#include <Udon/Traits/HasMemberFunctionUpdate.hpp>

struct A
{
    void begin()
    {
    }
    void show() const
    {
    }
    void update()
    {
    }
};

struct B
{
};

inline void test()
{
    static_assert(Udon::Traits::HasMemberFunctionShow<A>::value, "");
    static_assert(Udon::Traits::HasMemberFunctionUpdate<A>::value, "");
    static_assert(Udon::Traits::HasMemberFunctionBegin<A>::value, "");

    static_assert(not Udon::Traits::HasMemberFunctionShow<B>::value, "");
    static_assert(not Udon::Traits::HasMemberFunctionUpdate<B>::value, "");
    static_assert(not Udon::Traits::HasMemberFunctionBegin<B>::value, "");

    A a;
    Udon::Traits::MaybeInvokeBegin(a);
    Udon::Traits::MaybeInvokeShow(a);
    Udon::Traits::MaybeInvokeUpdate(a);

    B b;
    Udon::Traits::MaybeInvokeBegin(b);
    Udon::Traits::MaybeInvokeShow(b);
    Udon::Traits::MaybeInvokeUpdate(b);
}
