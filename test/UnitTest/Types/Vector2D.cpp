#include <gtest/gtest.h>

#include <Udon/Types/Vector2D.hpp>

TEST (Vector2D, Constructor)
{
    Udon::Vec2 v1;
    EXPECT_EQ(v1.x, 0.0);
    EXPECT_EQ(v1.y, 0.0);

    Udon::Vec2 v2(1.0, 2.0);
    EXPECT_EQ(v2.x, 1.0);
    EXPECT_EQ(v2.y, 2.0);
}