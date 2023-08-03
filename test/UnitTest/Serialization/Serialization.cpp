#include <gtest/gtest.h>
#include <Udon/Com/Serialization.hpp>

TEST(Serialization, huga)
{
	EXPECT_STRNE("hello", "world");
	EXPECT_EQ(7 * 6, 42);
}

TEST(Serialization, hoge)
{
	EXPECT_EQ(1, 1);
}