#include <Udon/Traits/HasMember.hpp>

namespace has_type
{

    UDON_HAS_MEMBER_TYPE(mType);

    struct TestStruct1
    {
        using mType = int;
    };
    struct TestStruct2
    {
    };

    void test()
    {
        static_assert(has_member_type_mType<TestStruct1>::value == true, "");
        static_assert(has_member_type_mType<TestStruct2>::value == false, "");
    }

}    // namespace has_type

namespace has_member_function
{

    UDON_HAS_MEMBER_FUNCTION(mFunc);

    struct TestStruct1
    {
        void mFunc() {}
    };
    struct TestStruct2
    {
    };

    void test()
    {
        static_assert(has_member_function_mFunc<TestStruct1>::value == true, "");
        static_assert(has_member_function_mFunc<TestStruct2>::value == false, "");
    }

}    // namespace has_member_function

namespace has_static_member_function
{

    UDON_HAS_STATIC_MEMBER_FUNCTION(mFunc);

    struct TestStruct1
    {
        static void mFunc() {}
    };
    struct TestStruct2
    {
    };

    void test()
    {
        static_assert(has_static_member_function_mFunc<TestStruct1>::value == true, "");
        static_assert(has_static_member_function_mFunc<TestStruct2>::value == false, "");
    }

}    // namespace has_static_member_function

namespace has_member_iterate_function
{

    UDON_HAS_MEMBER_ITERATE_FUNCTION(accessor);

    struct TestStruct1
    {
        int value;
        template <typename T>
        void accessor(T& acc)
        {
            acc(value);
        }
    };
    struct TestStruct2
    {
    };

    struct DummyMemberAccessor
    {
    };

    void test()
    {
        static_assert(has_member_iterate_accessor<DummyMemberAccessor, TestStruct1>::value == true, "");
        static_assert(has_member_iterate_accessor<DummyMemberAccessor, TestStruct2>::value == false, "");
    }

}    // namespace has_member_iterate_function
