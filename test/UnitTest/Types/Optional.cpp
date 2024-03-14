//
//    StringView Unit Test
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//


#include <gtest/gtest.h>
#include <Udon/Types/StringView.hpp>


struct Copyable
{
    int value;

    Copyable(int value = 0)
        : value(value){};

    ~Copyable() noexcept {};

    Copyable(const Copyable& other)
        : value(other.value){};

    Copyable(Copyable&&) noexcept = delete;

    Copyable& operator=(const Copyable& other)
    {
        value = other.value;
        return *this;
    };

    Copyable& operator=(Copyable&&) noexcept = delete;
};

struct CopyableDerived
{
    int value;

    CopyableDerived(int value = 0)
        : value(value){};

    ~CopyableDerived() noexcept {};

    CopyableDerived(const Copyable& other)
        : value(other.value)
    {
    }    // implicit conversion from Copyable to Copyable1

    CopyableDerived(CopyableDerived&&) noexcept = delete;

    CopyableDerived& operator=(const CopyableDerived& other)
    {
        value = other.value;
        return *this;
    };

    CopyableDerived& operator=(CopyableDerived&&) noexcept = delete;
};

struct Uncopyable
{
    int value;

    Uncopyable(int value = 0)
        : value(value){};

    ~Uncopyable() noexcept {};

    Uncopyable(const Uncopyable&) = delete;

    Uncopyable(Uncopyable&& other) noexcept
        : value(other.value){};

    Uncopyable& operator=(const Uncopyable&) = delete;

    Uncopyable& operator=(Uncopyable&& other) noexcept
    {
        value = other.value;
        return *this;
    };
};
struct UncopyableDerived
{

    int value;

    UncopyableDerived(int value = 0)
        : value(value){};

    ~UncopyableDerived() noexcept {};

    UncopyableDerived(const UncopyableDerived&) = delete;

    UncopyableDerived(Uncopyable&& other) noexcept
        : value(other.value)
    {
    }    // implicit conversion from Uncopyable to Uncopyable1

    UncopyableDerived& operator=(const UncopyableDerived&) = delete;

    UncopyableDerived& operator=(UncopyableDerived&& other) noexcept
    {
        value = other.value;
        return *this;
    };
};

void ConstructTest()
{
    // default construct
    {
        TestUdon::Optional<Copyable> a;
        EXPECT_FALSE(a);
    }
    {
        TestUdon::Optional<Copyable> a{ 100 };
        EXPECT_TRUE(a);
        EXPECT_EQ(a->value, 100);
    }

    // nullopt construct
    {
        TestUdon::Optional<Copyable> a{ TestUdon::nullopt };
        EXPECT_FALSE(a);
    }

    // copy construct
    {
        TestUdon::Optional<Copyable> a{ 100 };
        TestUdon::Optional<Copyable> b{ a };
        EXPECT_EQ(b->value, 100);
    }
    {
        Copyable a{ 100 };
        TestUdon::Optional<Copyable> b{ a };
        EXPECT_EQ(b->value, 100);
    }
    {
        // Copyable から Copyable1 への変換は可能なので構築可能
        TestUdon::Optional<Copyable>        a{ 100 };
        TestUdon::Optional<CopyableDerived> b{ a };
        EXPECT_EQ(b->value, 100);
    }
    {
        Copyable a{ 100 };
        TestUdon::Optional<CopyableDerived> b{ a };
        EXPECT_EQ(b->value, 100);
    }

    // move construct
    {
        TestUdon::Optional<Uncopyable> a{ 100 };
        TestUdon::Optional<Uncopyable> b{ std::move(a) };
        EXPECT_EQ(b->value, 100);
    }
    {
        // Uncopyable から Uncopyable1 への変換は可能なので構築可能
        Uncopyable                     a{ 100 };
        TestUdon::Optional<Uncopyable> b{ std::move(a) };
        EXPECT_EQ(b->value, 100);
    }
    {
        Uncopyable                            a{ 100 };
        TestUdon::Optional<UncopyableDerived> b{ std::move(a) };
        EXPECT_EQ(b->value, 100);
    }
    {
        TestUdon::Optional<Uncopyable>        a{ 100 };
        TestUdon::Optional<UncopyableDerived> b{ std::move(a) };
        EXPECT_EQ(b->value, 100);
    }
}


void AssignmentOperatorTest()
{
    // copy assingment
    {
        TestUdon::Optional<Copyable> a{ 100 };
        TestUdon::Optional<Copyable> b;
        b = a;
        EXPECT_EQ(b->value, 100);
    }
    {
        Copyable a{ 100 };
        TestUdon::Optional<Copyable> b;
        b = a;
        EXPECT_EQ(b->value, 100);
    }
    {
        TestUdon::Optional<Copyable>        a{ 100 };
        TestUdon::Optional<CopyableDerived> b;
        b = a;
        EXPECT_EQ(b->value, 100);
    }
    {
        Copyable a{ 100 };
        TestUdon::Optional<CopyableDerived> b;
        b = a;
        EXPECT_EQ(b->value, 100);
    }

    // move assingment
    {
        TestUdon::Optional<Uncopyable> a{ 100 };
        TestUdon::Optional<Uncopyable> b;
        b = std::move(a);
        EXPECT_EQ(b->value, 100);
    }
    {
        Uncopyable                     a{ 100 };
        TestUdon::Optional<Uncopyable> b;
        b = std::move(a);
        EXPECT_EQ(b->value, 100);
    }
    {
        TestUdon::Optional<Uncopyable>        a{ 100 };
        TestUdon::Optional<UncopyableDerived> b;
        b = std::move(a);
        EXPECT_EQ(b->value, 100);
    }
    {
        Uncopyable                            a{ 100 };
        TestUdon::Optional<UncopyableDerived> b;
        b = std::move(a);
        EXPECT_EQ(b->value, 100);
    }
}

void ValueAccessTest()
{
    {
        TestUdon::Optional<Copyable> a{ 100 };
        EXPECT_EQ(a.value().value, 100);    // value() &
        EXPECT_EQ((*a).value, 100);         // operator*() &
        EXPECT_EQ(a->value, 100);           // operator->() &
    }

    {
        const TestUdon::Optional<Copyable> a{ 100 };
        EXPECT_EQ(a.value().value, 100);    // value() const&
        EXPECT_EQ((*a).value, 100);         // operator*() const&
        EXPECT_EQ(a->value, 100);           // operator->() const&
    }

    {
        TestUdon::Optional<Copyable> a{ 100 };
        EXPECT_EQ(std::move(a).value().value, 100);    // value() &&
        EXPECT_EQ((*std::move(a)).value, 100);         // operator*() &&
        EXPECT_EQ(std::move(a)->value, 100);           // operator->() &&
    }

    {

#pragma warning(push)
#pragma warning(disable : 26478)    // 定数変数の移動は通常しないが、テストのため無効化

        const TestUdon::Optional<Copyable> a{ 100 };
        EXPECT_EQ(std::move(a).value().value, 100);    // value() const&&
        EXPECT_EQ((*std::move(a)).value, 100);         // operator*() const&&
        EXPECT_EQ(std::move(a)->value, 100);           // operator->() const&&

#pragma warning(pop)
    }
}

void CompairTest()
{
    // operator ==
    {
        TestUdon::Optional<int> a = 1;
        TestUdon::Optional<int> aa = 1;
        TestUdon::Optional<int> b = 2;
        TestUdon::Optional<int> n;

        // optional と optional
        EXPECT_TRUE(a == aa);
        EXPECT_TRUE(aa == a);
        EXPECT_FALSE(a == b);
        EXPECT_FALSE(b == a);

        // optional と nullopt
        EXPECT_FALSE(a == n);
        EXPECT_FALSE(n == a);
        EXPECT_FALSE(a == TestUdon::nullopt);
        EXPECT_FALSE(TestUdon::nullopt == a);

        // optional と T
        EXPECT_TRUE(a == 1);
        EXPECT_TRUE(1 == a);
        EXPECT_FALSE(a == 2);
        EXPECT_FALSE(2 == a);

        // nullopt と nullopt
        EXPECT_TRUE(n == TestUdon::nullopt);
        EXPECT_TRUE(TestUdon::nullopt == n);
    }

    // operator !=
    {
        TestUdon::Optional<int> a = 1;
        TestUdon::Optional<int> aa = 1;
        TestUdon::Optional<int> b = 2;
        TestUdon::Optional<int> n;

        // optional と optional
        EXPECT_FALSE(a != aa);
        EXPECT_FALSE(aa != a);
        EXPECT_TRUE(a != b);
        EXPECT_TRUE(b != a);

        // optional と nullopt
        EXPECT_TRUE(a != n);
        EXPECT_TRUE(n != a);
        EXPECT_TRUE(a != TestUdon::nullopt);
        EXPECT_TRUE(TestUdon::nullopt != a);

        // optional と T
        EXPECT_FALSE(a != 1);
        EXPECT_FALSE(1 != a);
        EXPECT_TRUE(a != 2);
        EXPECT_TRUE(2 != a);

        // nullopt と nullopt
        EXPECT_FALSE(n != TestUdon::nullopt);
        EXPECT_FALSE(TestUdon::nullopt != n);
    }

    // operator <
    {
        TestUdon::Optional<int> a = 1;
        TestUdon::Optional<int> aa = 1;
        TestUdon::Optional<int> b = 2;
        TestUdon::Optional<int> n;

        // optional と optional
        EXPECT_FALSE(a < aa);
        EXPECT_FALSE(aa < a);
        EXPECT_TRUE(a < b);
        EXPECT_FALSE(b < a);

        // optional と nullopt
        EXPECT_FALSE(a < n);
        EXPECT_TRUE(n < a);
        EXPECT_FALSE(a < TestUdon::nullopt);
        EXPECT_TRUE(TestUdon::nullopt < a);

        // optional と T
        EXPECT_FALSE(a < 1);
        EXPECT_FALSE(1 < a);
        EXPECT_TRUE(a < 2);
        EXPECT_FALSE(2 < a);

        // nullopt と nullopt
        EXPECT_FALSE(n < TestUdon::nullopt);
        EXPECT_FALSE(TestUdon::nullopt < n);
    }

    // operator >
    {
        TestUdon::Optional<int> a = 1;
        TestUdon::Optional<int> aa = 1;
        TestUdon::Optional<int> b = 2;
        TestUdon::Optional<int> n;

        // optional と optional
        EXPECT_FALSE(a > aa);
        EXPECT_FALSE(aa > a);
        EXPECT_FALSE(a > b);
        EXPECT_TRUE(b > a);

        // optional と nullopt
        EXPECT_TRUE(a > n);
        EXPECT_FALSE(n > a);
        EXPECT_TRUE(a > TestUdon::nullopt);
        EXPECT_FALSE(TestUdon::nullopt > a);

        // optional と T
        EXPECT_FALSE(a > 1);
        EXPECT_FALSE(1 > a);
        EXPECT_FALSE(a > 2);
        EXPECT_TRUE(2 > a);

        // nullopt と nullopt
        EXPECT_FALSE(n > TestUdon::nullopt);
        EXPECT_FALSE(TestUdon::nullopt > n);
    }

    // operator <=
    {
        TestUdon::Optional<int> a = 1;
        TestUdon::Optional<int> aa = 1;
        TestUdon::Optional<int> b = 2;
        TestUdon::Optional<int> n;

        // optional と optional
        EXPECT_TRUE(a <= aa);
        EXPECT_TRUE(aa <= a);
        EXPECT_TRUE(a <= b);
        EXPECT_FALSE(b <= a);

        // optional と nullopt
        EXPECT_FALSE(a <= n);
        EXPECT_TRUE(n <= a);
        EXPECT_FALSE(a <= TestUdon::nullopt);
        EXPECT_TRUE(TestUdon::nullopt <= a);

        // optional と T
        EXPECT_TRUE(a <= 1);
        EXPECT_TRUE(1 <= a);
        EXPECT_TRUE(a <= 2);
        EXPECT_FALSE(2 <= a);

        // nullopt と nullopt
        EXPECT_TRUE(n <= TestUdon::nullopt);
        EXPECT_TRUE(TestUdon::nullopt <= n);
    }

    // operator >=
    {
        TestUdon::Optional<int> a = 1;
        TestUdon::Optional<int> aa = 1;
        TestUdon::Optional<int> b = 2;
        TestUdon::Optional<int> n;

        // optional と optional
        EXPECT_TRUE(a >= aa);
        EXPECT_TRUE(aa >= a);
        EXPECT_FALSE(a >= b);
        EXPECT_TRUE(b >= a);

        // optional と nullopt
        EXPECT_TRUE(a >= n);
        EXPECT_FALSE(n >= a);
        EXPECT_TRUE(a >= TestUdon::nullopt);
        EXPECT_FALSE(TestUdon::nullopt >= a);

        // optional と T
        EXPECT_TRUE(a >= 1);
        EXPECT_TRUE(1 >= a);
        EXPECT_FALSE(a >= 2);
        EXPECT_TRUE(2 >= a);

        // nullopt と nullopt
        EXPECT_TRUE(n >= TestUdon::nullopt);
        EXPECT_TRUE(TestUdon::nullopt >= n);
    }
}

void ValueOrTest()
{
    {
        TestUdon::Optional<int> a = 1;
        EXPECT_EQ(a.valueOr(2), 1);
    }

    {
        TestUdon::Optional<int> a;
        EXPECT_EQ(a.valueOr(2), 2);
    }

    {
        TestUdon::Optional<Copyable> copyable;
        EXPECT_EQ(copyable.valueOr(100).value, 100);    //  const&
    }

    {
        EXPECT_EQ(TestUdon::Optional<Uncopyable>{ TestUdon::nullopt }.valueOr(100).value, 100);    // &&
    }
}

void ResetTest()
{
    TestUdon::Optional<Copyable> a{ 100 };
    a.reset();
    EXPECT_FALSE(a);
}
