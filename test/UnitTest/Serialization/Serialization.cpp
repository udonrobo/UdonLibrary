#include <gtest/gtest.h>

#include <Udon/Serializer/Serializer.hpp>

struct Vec2
{
    double x, y;
    bool operator==(const Vec2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vec2& rhs) const { return !(*this == rhs); }
    UDON_ENUMERABLE(x, y);
};

TEST(Serialization, BufferSizeAfterSerialization)
{
    EXPECT_EQ(Udon::SerializedSize<bool    >(), 1     + 1);
    EXPECT_EQ(Udon::SerializedSize<uint8_t >(), 1     + 1);
    EXPECT_EQ(Udon::SerializedSize<uint16_t>(), 2     + 1);
    EXPECT_EQ(Udon::SerializedSize<uint32_t>(), 4     + 1);
    EXPECT_EQ(Udon::SerializedSize<uint64_t>(), 8     + 1);
    EXPECT_EQ(Udon::SerializedSize<float   >(), 4     + 1);
    EXPECT_EQ(Udon::SerializedSize<double  >(), 4     + 1);
    EXPECT_EQ(Udon::SerializedSize<Vec2    >(), 4 + 4 + 1);
}

TEST(Serialization, SerializeDeserialize)
{
	Vec2 v{ 1.0, 2.0 };

    const auto serialized = Udon::Serialize(v);
    EXPECT_EQ(Udon::SerializedSize<Vec2>(), serialized.size());    // シリアライズ後のサイズが正しいか確認
    EXPECT_EQ(serialized.back(), Udon::CRC8({ serialized.data(), serialized.size() - 1 }));    // CRCが正しいか確認

    const auto deserialized = Udon::Deserialize<Vec2>(serialized);
    EXPECT_TRUE(deserialized.has_value());    // デシリアライズに成功したか確認
    EXPECT_EQ(v, deserialized.value());       // デシリアライズ後の値が正しいか確認
}

TEST(Serialization, DeserializeError)
{
    Vec2 v{ 1.0, 2.0 };
    auto serialized = Udon::Serialize(v);

    serialized[2] = 0x00;  // CRCが不正になるように変更

    const auto deserialized = Udon::Deserialize<Vec2>(serialized);

    EXPECT_FALSE(deserialized.has_value());
    EXPECT_NE(v, deserialized.value());
}
