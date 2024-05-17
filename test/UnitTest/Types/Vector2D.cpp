//
//    ユニットテスト
// 
//    Copyright (c) 2022-2024 udonrobo
//

#include <gtest/gtest.h>

#include <Udon/Types/Vector2D.hpp>

TEST(Vec2, Constructor)
{
    Udon::Vec2 v1;
    EXPECT_EQ(v1.x, 0.0);
    EXPECT_EQ(v1.y, 0.0);

    Udon::Vec2 v2(1.0, 2.0);
    EXPECT_EQ(v2.x, 1.0);
    EXPECT_EQ(v2.y, 2.0);
}

TEST(Vec2, Operator)
{
    Udon::Vec2 v1(1.0, 2.0);
    Udon::Vec2 v2(3.0, 4.0);

    Udon::Vec2 v3 = v1 + v2;
    EXPECT_EQ(v3.x, 4.0);
    EXPECT_EQ(v3.y, 6.0);

    Udon::Vec2 v4 = v1 - v2;
    EXPECT_EQ(v4.x, -2.0);
    EXPECT_EQ(v4.y, -2.0);

    Udon::Vec2 v5 = v1 * 2.0;
    EXPECT_EQ(v5.x, 2.0);
    EXPECT_EQ(v5.y, 4.0);

    Udon::Vec2 v6 = v1 / 2.0;
    EXPECT_EQ(v6.x, 0.5);
    EXPECT_EQ(v6.y, 1.0);

    Udon::Vec2 v8 = v1;
    EXPECT_EQ(v8.x, 1.0);
    EXPECT_EQ(v8.y, 2.0);

    v8 += v2;
    EXPECT_EQ(v8.x, 4.0);
    EXPECT_EQ(v8.y, 6.0);

    v8 -= v2;
    EXPECT_EQ(v8.x, 1.0);
    EXPECT_EQ(v8.y, 2.0);

    v8 *= 2.0;
    EXPECT_EQ(v8.x, 2.0);
    EXPECT_EQ(v8.y, 4.0);

    v8 /= 2.0;
    EXPECT_EQ(v8.x, 1.0);
    EXPECT_EQ(v8.y, 2.0);
}

#include <cmath>

TEST (Vec2, Method)
{
    Udon::Vec2 v1(1.0, 2.0);
    Udon::Vec2 v2(3.0, 4.0);

    EXPECT_DOUBLE_EQ(v1.length(), std::sqrt(5.0));

    EXPECT_DOUBLE_EQ(v1.distanceFrom(v2), std::sqrt(8.0));

    EXPECT_FALSE(v1.isZero());
    EXPECT_TRUE(Udon::Vec2(0.0, 0.0).isZero());
    
}